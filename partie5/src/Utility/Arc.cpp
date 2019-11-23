/*
 * infosv
 * jan 2013
 * Marco Antognini
 */

#include "Arc.hpp"
#include "../Utility/Utility.hpp"

Arc::Arc(double start, double end, double radius, sf::Color color, float thickness, unsigned int pointCount)
    : mVertices(sf::PrimitiveType::TrianglesStrip)
    , mStart(start)
    , mEnd(end)
    , mRadius(radius)
    , mColor(color)
    , mThickness(thickness)
    , mPointCount(pointCount)
{
    update();
}

sf::FloatRect Arc::getLocalBounds() const
{
    return mVertices.getBounds();
}

sf::FloatRect Arc::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

double Arc::getStart() const
{
    return mStart;
}

void Arc::setStart(double start)
{
    mStart = start;
    update();
}

double Arc::getEnd() const
{
    return mEnd;
}

void Arc::setEnd(double end)
{
    mEnd = end;
    update();
}

double Arc::getRadius() const
{
    return mRadius;
}

void Arc::setRadius(double radius)
{
    mRadius = radius;
    update();
}

sf::Color Arc::getColor() const
{
    return mColor;
}

void Arc::setColor(sf::Color color)
{
    mColor = color;
    updateColorOnly();
}

float Arc::getThickness() const
{
    return mThickness;
}

void Arc::setThickness(float thickness)
{
    mThickness = thickness;
    update();
}

void Arc::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mVertices, states);
}

void Arc::update()
{
    // Compute the normalised normal of [a, b] segment.
    // Note: we do it here just to reduce a little bit the inter-dependencies.
    auto const normalisedNormal = [](sf::Vector2f const& a, sf::Vector2f const& b) -> sf::Vector2f {
        sf::Vector2f const v = a - b;
        sf::Vector2f const n = { v.y, -v.x };
        float const length = std::sqrt(n.x * n.x + n.y * n.y);

        return length != 0.0 ? n / length : n;
    };

    // Fonction used to compute each point
    auto const f = [this](double t) -> sf::Vector2f {
        return {
            (float)(std::cos(t * DEG_TO_RAD) * mRadius + mRadius),
            (float)(std::sin(t * DEG_TO_RAD) * mRadius + mRadius)
        };
        // «+ mRadius» because we compute from the top left corner like all SFML shapes.
    };

    mVertices.clear();
    mVertices.resize(mPointCount * 2 + 1); // (count + 1) points for the curve and count points for the outline
    // (That is, one point for the outline between each point of the curve.)
    // (We need one more point on the curve to include both extremes.)

    // Compute the points of the curve
    float const step = (mEnd - mStart) / mPointCount;
    for (unsigned int i = 0; i <= mPointCount; ++i) { // from begin to end, inclusive, so count + 1 steps
        float const t = mStart + step * i;
        mVertices[i * 2] = { f(t), mColor }; // one vertex over two because of the outline
    }

    // Compute the points of the outline
    for (unsigned int i = 1; i < mPointCount * 2; i += 2) {
        // Compute the normal of the two points (p0 and p2) of the curve that are next to this outline point (p1)
        sf::Vector2f const& p0 = mVertices[i - 1].position;
        sf::Vector2f const& p2 = mVertices[i + 1].position;
        sf::Vector2f const  n  = normalisedNormal(p0, p2);
        sf::Vector2f const  m  = (p0 + p2) / 2.f;
        sf::Vector2f const  p1 = m + n * mThickness;

        mVertices[i] = { p1, mColor };
    }

    // Now, let's be clever !
    //
    // If the first and last points are very close
    // then we add another point for the outline in order to "link" them together.
    sf::Vector2f const& begin = mVertices[0].position;
    sf::Vector2f const& end   = mVertices[mVertices.getVertexCount() - 1].position;
    sf::Vector2f const  delta = end - begin;
    float const length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length <= 0.1f) { // Close enough !
        // This last outline point is computed from the first and last outline points
        // because we can't compute a normal vector between begin and end
        // (it might well be zero).
        sf::Vector2f const& first = mVertices[1].position;
        sf::Vector2f const& last  = mVertices[mVertices.getVertexCount() - 2].position;
        sf::Vector2f const  mid   = (first + last) / 2.f;

        mVertices.append({ mid, mColor });

        // And to make things even prettier we add the first outline point again to end the loop
        mVertices.append(mVertices[1]);
    }
}

void Arc::updateColorOnly()
{
    for (unsigned int i = 0; i < mVertices.getVertexCount(); ++i) {
        mVertices[i].color = mColor;
    }
}

