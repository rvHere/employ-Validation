#include "trainWindRestriction.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

// --- Private helpers ---

double TrainWindRestrictionEvaluator::milepostGapToFeet(double mp1, double mp2) {
    return std::abs(mp2 - mp1) * FEET_PER_MILE;
}

const WindWarning* TrainWindRestrictionEvaluator::findNextEqualOrLesserWarning(
    const WindWarning& current,
    const std::vector<WindWarning>& warningsAhead
) {
    const WindWarning* nearest = nullptr;
    double nearestStart = std::numeric_limits<double>::max();

    for (const auto& w : warningsAhead) {
        // Warning must be ahead of the current warning's end
        if (w.startMilepost > current.endMilepost &&
            w.windSpeedMph <= current.windSpeedMph &&
            w.startMilepost < nearestStart) {
            nearest = &w;
            nearestStart = w.startMilepost;
        }
    }
    return nearest;
}

bool TrainWindRestrictionEvaluator::hasSufficientStopSpace(
    double gapFeet, double trainLengthFeet
) {
    // The train needs at least its own length of clear track to come to a
    // complete stop between the two warning zones.  If the gap is shorter
    // than the train length, the train physically cannot fit between the
    // warnings and therefore cannot stop in the gap.
    return gapFeet >= trainLengthFeet;
}

FODMessage TrainWindRestrictionEvaluator::buildFODMessage(
    const Train& train,
    const Block& nextBlock,
    const WindWarning& nextWarning
) {
    std::ostringstream directive;
    directive << "Train " << train.trainId
              << " authorized to enter Block " << nextBlock.blockId
              << " (MP " << std::fixed << std::setprecision(1)
              << nextBlock.startMilepost << " - " << nextBlock.endMilepost << ")"
              << " at " << WIND_RESTRICTION_SPEED_MPH << " MPH."
              << " Wind restriction ahead: " << nextWarning.windSpeedMph << " MPH wind"
              << " from MP " << nextWarning.startMilepost
              << " to MP " << nextWarning.endMilepost << "."
              << " Insufficient gap for stop — BLI issued for continuous movement.";

    FODMessage fod;
    fod.messageId = nextFodMessageId++;
    fod.trainId = train.trainId;
    fod.targetBlockId = nextBlock.blockId;
    fod.speedLimitMph = WIND_RESTRICTION_SPEED_MPH;
    fod.windSpeedMph = nextWarning.windSpeedMph;
    fod.directive = directive.str();
    fod.timestamp = std::time(nullptr);
    return fod;
}

// --- Public API ---

WindRestrictionRuleResult TrainWindRestrictionEvaluator::evaluate(
    const Train& train,
    const WindWarning& currentWarning,
    const std::vector<WindWarning>& warningsAhead,
    const Block& currentBlock,
    const Block& nextBlock
) {
    WindRestrictionRuleResult result{};
    result.trainLengthFeet = train.lengthFeet;
    result.canContinue = false;
    result.hasWarningAhead = false;
    result.sufficientStopSpace = true;
    result.gapDistanceFeet = 0.0;

    // Precondition: train must be traveling at the wind restriction speed (10 mph)
    if (train.currentSpeedMph != WIND_RESTRICTION_SPEED_MPH) {
        result.reason = "Train is not currently traveling at "
                      + std::to_string(WIND_RESTRICTION_SPEED_MPH)
                      + " MPH wind restriction speed.";
        return result;
    }

    // Precondition: current warning must impose a 10 MPH speed limit
    if (currentWarning.speedLimitMph != WIND_RESTRICTION_SPEED_MPH) {
        result.reason = "Current warning does not impose a "
                      + std::to_string(WIND_RESTRICTION_SPEED_MPH)
                      + " MPH restriction.";
        return result;
    }

    // Find the next warning ahead with equal or lesser wind speed
    const WindWarning* nextWarning =
        findNextEqualOrLesserWarning(currentWarning, warningsAhead);

    if (!nextWarning) {
        result.reason = "No warning ahead with equal or lesser wind speed found.";
        return result;
    }

    result.hasWarningAhead = true;

    // Calculate the gap between the end of the current warning and the start
    // of the next warning, converted to feet
    double gapFeet = milepostGapToFeet(currentWarning.endMilepost,
                                        nextWarning->startMilepost);
    result.gapDistanceFeet = gapFeet;

    // Determine if the train has enough space to stop in the gap
    result.sufficientStopSpace = hasSufficientStopSpace(gapFeet, train.lengthFeet);

    if (result.sufficientStopSpace) {
        // There IS enough room — normal procedures apply; no BLI needed
        result.reason = "Gap of " + std::to_string(static_cast<int>(gapFeet))
                      + " ft between warnings is sufficient for train ("
                      + std::to_string(static_cast<int>(train.lengthFeet))
                      + " ft) to stop. Standard procedures apply.";
        return result;
    }

    // Not enough space to stop between warnings — deliver BLI
    result.canContinue = true;
    result.reason = "Gap of " + std::to_string(static_cast<int>(gapFeet))
                  + " ft is less than train length of "
                  + std::to_string(static_cast<int>(train.lengthFeet))
                  + " ft. BLI issued — train may continue at "
                  + std::to_string(WIND_RESTRICTION_SPEED_MPH)
                  + " MPH into next block.";
    return result;
}

BLI TrainWindRestrictionEvaluator::generateBLI(
    const Train& train,
    const WindWarning& currentWarning,
    const WindWarning& nextWarning,
    const Block& currentBlock,
    const Block& nextBlock
) {
    BLI bli;
    bli.bliId = nextBliId++;
    bli.trainId = train.trainId;
    bli.currentBlockId = currentBlock.blockId;
    bli.nextBlockId = nextBlock.blockId;
    bli.authorizedSpeedMph = WIND_RESTRICTION_SPEED_MPH;
    bli.associatedFOD = buildFODMessage(train, nextBlock, nextWarning);
    return bli;
}

// --- Demo / main ---

static void printRuleResult(const WindRestrictionRuleResult& r) {
    std::cout << "\n===== Wind Restriction Rule Evaluation =====\n";
    std::cout << "Warning ahead found   : " << (r.hasWarningAhead ? "Yes" : "No") << "\n";
    std::cout << "Gap distance (ft)     : " << static_cast<int>(r.gapDistanceFeet) << "\n";
    std::cout << "Train length (ft)     : " << static_cast<int>(r.trainLengthFeet) << "\n";
    std::cout << "Sufficient stop space : " << (r.sufficientStopSpace ? "Yes" : "No") << "\n";
    std::cout << "BLI to be delivered   : " << (r.canContinue ? "Yes" : "No") << "\n";
    std::cout << "Reason                : " << r.reason << "\n";
}

static void printBLI(const BLI& bli) {
    std::cout << "\n----- BLI Delivered -----\n";
    std::cout << "BLI ID             : " << bli.bliId << "\n";
    std::cout << "Train              : " << bli.trainId << "\n";
    std::cout << "Current Block      : " << bli.currentBlockId << "\n";
    std::cout << "Next Block         : " << bli.nextBlockId << "\n";
    std::cout << "Authorized Speed   : " << bli.authorizedSpeedMph << " MPH\n";
    std::cout << "\n  -- Updated FOD Message --\n";
    std::cout << "  FOD ID           : " << bli.associatedFOD.messageId << "\n";
    std::cout << "  Target Block     : " << bli.associatedFOD.targetBlockId << "\n";
    std::cout << "  Speed Limit      : " << bli.associatedFOD.speedLimitMph << " MPH\n";
    std::cout << "  Wind Speed       : " << bli.associatedFOD.windSpeedMph << " MPH\n";
    std::cout << "  Directive        : " << bli.associatedFOD.directive << "\n";
}

int main() {
    TrainWindRestrictionEvaluator evaluator;

    // --- Scenario 1: Train too long to stop between warnings → BLI issued ---
    std::cout << "========================================\n";
    std::cout << " SCENARIO 1: Train cannot stop in gap\n";
    std::cout << "========================================\n";
    {
        Train train{"BNSF-4521", 7200.0, 10, 45.0};  // 7200 ft train at MP 45

        WindWarning currentWarn{1, 44.0, 46.0, 55.0, 10};  // MP 44–46, 55 mph wind
        WindWarning aheadWarn{2, 47.0, 49.0, 50.0, 10};    // MP 47–49, 50 mph wind (lesser)

        Block currentBlk{101, 43.0, 46.5};
        Block nextBlk{102, 46.5, 50.0};

        std::vector<WindWarning> warningsAhead = {aheadWarn};

        auto result = evaluator.evaluate(train, currentWarn, warningsAhead,
                                         currentBlk, nextBlk);
        printRuleResult(result);

        if (result.canContinue) {
            auto bli = evaluator.generateBLI(train, currentWarn, aheadWarn,
                                             currentBlk, nextBlk);
            printBLI(bli);
        }
    }

    std::cout << "\n";

    // --- Scenario 2: Enough room to stop — no BLI needed ---
    std::cout << "========================================\n";
    std::cout << " SCENARIO 2: Sufficient gap to stop\n";
    std::cout << "========================================\n";
    {
        Train train{"UP-9083", 3000.0, 10, 20.0};  // 3000 ft train at MP 20

        WindWarning currentWarn{3, 19.0, 21.0, 60.0, 10};  // MP 19–21, 60 mph wind
        WindWarning aheadWarn{4, 25.0, 27.0, 55.0, 10};    // MP 25–27, 55 mph wind

        Block currentBlk{201, 18.0, 22.0};
        Block nextBlk{202, 22.0, 28.0};

        std::vector<WindWarning> warningsAhead = {aheadWarn};

        auto result = evaluator.evaluate(train, currentWarn, warningsAhead,
                                         currentBlk, nextBlk);
        printRuleResult(result);

        if (result.canContinue) {
            std::cout << "(BLI would be delivered here)\n";
        } else {
            std::cout << "\nNo BLI needed — standard stop procedures apply.\n";
        }
    }

    std::cout << "\n";

    // --- Scenario 3: Warning ahead has higher wind speed — rule does not apply ---
    std::cout << "========================================\n";
    std::cout << " SCENARIO 3: Next warning has higher wind\n";
    std::cout << "========================================\n";
    {
        Train train{"CSX-1122", 5000.0, 10, 30.0};

        WindWarning currentWarn{5, 29.0, 31.0, 45.0, 10};
        WindWarning aheadWarn{6, 32.0, 34.0, 65.0, 10};  // 65 > 45, higher wind

        Block currentBlk{301, 28.0, 31.5};
        Block nextBlk{302, 31.5, 35.0};

        std::vector<WindWarning> warningsAhead = {aheadWarn};

        auto result = evaluator.evaluate(train, currentWarn, warningsAhead,
                                         currentBlk, nextBlk);
        printRuleResult(result);
    }

    return 0;
}
