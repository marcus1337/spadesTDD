#include "spades/data/Score.h"
#include "spades/pch.h"

using namespace spd;

int Score::getRoundTrickPoints(const SeatRoundResult &r1, const SeatRoundResult &r2) const
{
    const bool hitTarget = (r1.bid + r2.bid) <= (r1.getCountedTricks() + r2.getCountedTricks());
    const int r1PPT = r1.getPointsPerTrick(scoreSettings);
    const int r2PPT = r2.getPointsPerTrick(scoreSettings);
    const int points = r1.getPointsPerTrick(scoreSettings) * r1.bid + r2.getPointsPerTrick(scoreSettings) * r2.bid;
    return hitTarget ? points : -points;
}

int Score::getRoundPoints(const SeatRoundResult &r1, const SeatRoundResult &r2) const
{
    return (r1.getNilScore(scoreSettings) + r2.getNilScore(scoreSettings)) + (getRoundBags(r1, r2) * scoreSettings.getPointsPerBag()) + getRoundTrickPoints(r1, r2);
}
int Score::getRoundBags(const SeatRoundResult &r1, const SeatRoundResult &r2) const
{
    const int countedTricks = r1.getCountedTricks() + r2.getCountedTricks();
    const int teamBid = r1.bid + r2.bid;
    if (countedTricks > teamBid)
    {
        return countedTricks - teamBid;
    }
    return 0;
}

Score::Score(const ScoreSettings &scoreSettings, const std::pair<Seat, Seat> &team, const std::vector<std::vector<Seat>> &completedRoundTrickTakers, const std::vector<std::array<std::pair<Seat, int>, NUM_SEATS>> &completedRoundBids, const std::vector<std::set<std::pair<Seat, BidOption>>> &completedRoundBidOptions) : scoreSettings(scoreSettings)
{
    assert(team.first != team.second);
    assert(completedRoundTrickTakers.size() == completedRoundBids.size());
    assert(completedRoundTrickTakers.size() == completedRoundBidOptions.size());
    const int numRounds = completedRoundTrickTakers.size();
    for (int round = 0; round < numRounds; round++)
    {
        const SeatRoundResult sr1(team.first, round, completedRoundTrickTakers, completedRoundBids, completedRoundBidOptions);
        const SeatRoundResult sr2(team.first, round, completedRoundTrickTakers, completedRoundBids, completedRoundBidOptions);
        roundPoints.push_back(getRoundPoints(sr1, sr2));
        roundBags.push_back(getRoundBags(sr1, sr2));
    }
}

std::vector<int> Score::getRoundBags() const
{
    return roundBags;
}

std::vector<int> Score::getRoundPoints() const
{
    return roundPoints;
}

int Score::getBags() const
{
    return scoreSettings.getBags(getTotalBags());
}

int Score::getTotalBags() const
{
    return std::accumulate(roundBags.begin(), roundBags.end(), 0);
}

int Score::getNumBagSets() const
{
    return scoreSettings.getNumBagSets(getTotalBags());
}

int Score::getPoints() const
{
    const int bagSetPenalty = scoreSettings.getBagSetPenalty();
    const int points = std::accumulate(roundPoints.begin(), roundPoints.end(), 0);
    return points - getNumBagSets() * bagSetPenalty;
}

ScoreResult Score::getScoreResult(const ScoreSettings &scoreSettings, const Score &score1, const Score &score2)
{
    const auto aPoints = score1.getPoints();
    const auto bPoints = score2.getPoints();
    const auto winScore = scoreSettings.getWinScore();
    const auto loseScore = scoreSettings.getLoseScore();

    if (aPoints < winScore && bPoints < winScore &&
        aPoints > loseScore && bPoints > loseScore)
    {
        return ScoreResult::ONGOING;
    }
    if (aPoints > bPoints)
    {
        return ScoreResult::TEAM_A_WIN;
    }
    else if (bPoints > aPoints)
    {
        return ScoreResult::TEAM_B_WIN;
    }
    else
    {
        return ScoreResult::ONGOING;
    }
}