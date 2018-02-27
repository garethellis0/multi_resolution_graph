#ifndef THUNDERBOTS_NAVIGATOR_WORLD_H
#define THUNDERBOTS_NAVIGATOR_WORLD_H

/**
 * This is the world class used by Thunderbots
 * It's here so we can mock it
 */

class World final
{
public:
    /**
     * \brief Constructs a new World
     *
     * \param[in] impl the backend implementation
     */
    explicit World(AI::BE::Backend &impl);

    /**
     * \brief Constructs a copy of a World
     *
     * \param[in] copyref the object to copy
     */
    World(const World &copyref);

    /**
     * \brief Compares two world for equality.
     *
     * \param[in] w the world to compare to
     *
     * \return \c true if this world is equal to \p w, or \c false if not
     */
    bool operator==(const World &w) const;

    /**
     * \brief Returns the field
     *
     * \return the field
     */
    const Field &field() const;

    /**
     * \brief Returns the ball
     *
     * \return the ball
     */
    Ball ball() const;

    /**
     * \brief Returns the friendly team
     *
     * \return the friendly team
     */
    FriendlyTeam friendly_team() const;

    /**
     * \brief Returns the enemy team
     *
     * \return the enemy team
     */
    EnemyTeam enemy_team() const;

    /**
     * \brief Returns the current play type
     *
     * \return the current play type
     */
    const Property<AI::Common::PlayType> &playtype() const;

    /**
     * \brief returns the ball placement position
     */
    const Property<Point> &ball_placement_position() const;

    /**
     * \brief Returns the current monotonic time
     *
     * Monotonic time is a way of representing "game time", which always moves
     * forward
     * Monotonic time is consistent within the game world, and may or may not be
     * linked to real time
     * A navigator should \em always use this function to retrieve monotonic
     * time!
     * The AI will not generally have any use for real time
     *
     * \return the current monotonic time
     */
    Timestamp monotonic_time() const;

private:
    AI::BE::Backend &impl;
};


#endif //THUNDERBOTS_NAVIGATOR_WORLD_H
