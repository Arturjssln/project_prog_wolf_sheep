
#ifndef __INFOSV_ARC_HPP__
#define __INFOSV_ARC_HPP__

#include <SFML/Graphics.hpp>
#include <cmath>

/*!
 * @class Arc
 *
 * @brief Arc is similar to sf::CircularShape expect that it can draw partial circle.
 *
 * Here is an example, displaying an arc from 45° to 135° with a radius of 100px :
 *
 * @code
 *
 * Arc arcgraphics(45, 135, 100, sf::Color::Black, 3.f);
 * arcgraphics.setOrigin(radius, radius);
 * arcgraphics.setPosition(mOrigin);
 * target.draw(arcgraphics);
 *
 * @endcode
 */
class Arc : public sf::Drawable,
            public sf::Transformable
{
public:
    /*!
     * @brief Constructor
     *
     * @param start start angle of the arc, in degree
     * @param end end angle of the arc, in degree
     * @param radius radius of the arc
     * @param color curve's color
     * @param thickness curve's thickness
     */
    Arc(double start, double end, double radius, sf::Color color = sf::Color::Black, float thickness = 1.f, unsigned int pointCount = 120);

    /*!
     * @brief Virtual destructor
     */
    virtual ~Arc()  { };

    /*!
     * @brief Get the local bounding rectangle of the entity
     *
     * The returned rectangle is in local coordinates, which means
     * that it ignores the transformations (translation, rotation,
     * scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * entity in the entity's coordinate system.
     *
     * @return Local bounding rectangle of the entity
     */
    sf::FloatRect getLocalBounds() const;

    /*!
     * @brief Get the global bounding rectangle of the entity
     *
     * The returned rectangle is in global coordinates, which means
     * that it takes in account the transformations (translation,
     * rotation, scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * sprite in the global 2D world's coordinate system.
     *
     * @return Global bounding rectangle of the entity
     */
    sf::FloatRect getGlobalBounds() const;

    /*!
     * @brief Get start angle
     *
     * @return start angle
     */
    double getStart() const;

    /*!
     * @brief Set start angle
     *
     * @param start new start angle in degree
     */
    void setStart(double start);

    /*!
     * @brief Get end angle
     *
     * @return end angle
     */
    double getEnd() const;

    /*!
     * @brief Set end angle
     *
     * @param end new end angle in degree
     */
    void setEnd(double end);

    /*!
     * @brief Get the arc's radius
     *
     * @return arc's radius
     */
    double getRadius() const;

    /*!
     * @brief Set the arc's radius
     *
     * @param radius new arc's radius
     */
    void setRadius(double radius);

    /*!
     * @brief Get the curve's color
     *
     * @return curve's color
     */
    sf::Color getColor() const;

    /*!
     * @brief Set the curve's color
     *
     * @param color new curve's color
     */
    void setColor(sf::Color color);

    /*!
     * @brief Get the curve's thickness
     *
     * @return curve's thickness
     */
    float getThickness() const;

    /*!
     * @brief Set the curve's thickness
     *
     * @param thickness new curve's thickness
     */
    void setThickness(float thickness);

protected:
    /*!
     * @brief Draw the curve to a render target
     *
     * @param target Render target to draw to
     * @param states Current render states
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    /*!
     * @brief Update vertices cache.
     */
    void update();

    /*!
     * @brief Update vertices' color only
     *
     * No need to recompute every points.
     */
    void updateColorOnly();

private:
    sf::VertexArray mVertices; ///< Vertices
    double mStart; ///< start angle, in degree
    double mEnd; ///< end angle, in degree
    double mRadius; ///< Displayed radius
    sf::Color mColor; ///< curve's color
    float mThickness; ///< curve's thickness
    unsigned int mPointCount;
};

#endif // __INFOSV_ARC_HPP__
