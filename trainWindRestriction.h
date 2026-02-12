#ifndef TRAIN_WIND_RESTRICTION_H
#define TRAIN_WIND_RESTRICTION_H

#include <string>
#include <vector>
#include <ctime>

// Represents a wind warning applied to a specific block/milepost range
struct WindWarning {
    int warningId;
    double startMilepost;   // where the warning zone begins
    double endMilepost;     // where the warning zone ends
    double windSpeedMph;    // observed/forecast wind speed in mph
    int speedLimitMph;      // restricted train speed (typically 10 mph)
};

// Represents a block of track
struct Block {
    int blockId;
    double startMilepost;
    double endMilepost;
};

// Train properties
struct Train {
    std::string trainId;
    double lengthFeet;       // total train length in feet
    double currentSpeedMph;
    double headPositionMilepost;  // front of train position
};

// FOD (Field Operations Directive) message sent to the train
struct FODMessage {
    int messageId;
    std::string trainId;
    int targetBlockId;
    int speedLimitMph;
    double windSpeedMph;
    std::string directive;   // human-readable directive text
    std::time_t timestamp;
};

// BLI (Block Limit Instruction) authorizing entry into the next block
struct BLI {
    int bliId;
    std::string trainId;
    int currentBlockId;
    int nextBlockId;
    int authorizedSpeedMph;
    FODMessage associatedFOD;
};

// Result of evaluating the wind restriction continuation rule
struct WindRestrictionRuleResult {
    bool canContinue;           // true if BLI should be delivered
    bool hasWarningAhead;       // next warning exists with equal/lesser wind
    double gapDistanceFeet;     // distance between warnings
    double trainLengthFeet;     // train length for reference
    bool sufficientStopSpace;   // whether train can stop in the gap
    std::string reason;         // explanation of the decision
};

// Core evaluation logic
class TrainWindRestrictionEvaluator {
public:
    // Evaluate whether a train under a wind restriction can receive a BLI
    // to continue into the next block at 10 mph without stopping.
    //
    // Rule: If the train is currently traveling at 10 mph in a wind restriction,
    // and the next warning ahead has equal or lesser wind speed, check if
    // there is enough space between the two warnings for the train to stop.
    // If there is NOT enough space (gap < train length), deliver a BLI with
    // an updated FOD message so the train can continue at 10 mph.
    WindRestrictionRuleResult evaluate(
        const Train& train,
        const WindWarning& currentWarning,
        const std::vector<WindWarning>& warningsAhead,
        const Block& currentBlock,
        const Block& nextBlock
    );

    // Generate a BLI with an updated FOD message for the train
    BLI generateBLI(
        const Train& train,
        const WindWarning& currentWarning,
        const WindWarning& nextWarning,
        const Block& currentBlock,
        const Block& nextBlock
    );

private:
    static constexpr double FEET_PER_MILE = 5280.0;
    static constexpr int WIND_RESTRICTION_SPEED_MPH = 10;

    // Convert milepost distance to feet
    static double milepostGapToFeet(double mp1, double mp2);

    // Find the nearest warning ahead with equal or lesser wind speed
    const WindWarning* findNextEqualOrLesserWarning(
        const WindWarning& current,
        const std::vector<WindWarning>& warningsAhead
    );

    // Determine if the gap between two warnings is sufficient for the train to stop
    bool hasSufficientStopSpace(double gapFeet, double trainLengthFeet);

    // Build the FOD message for BLI delivery
    FODMessage buildFODMessage(
        const Train& train,
        const Block& nextBlock,
        const WindWarning& nextWarning
    );

    // Internal counter for generating unique IDs
    int nextBliId = 1;
    int nextFodMessageId = 1;
};

#endif // TRAIN_WIND_RESTRICTION_H
