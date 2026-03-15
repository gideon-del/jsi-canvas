# Winding Numbers in Paths

Recently I've been doing some research on vector graphics and decided to build a vector engine from scratch to further this. I got to the point of building compound paths (A list of Paths). Then I thought of something, How would I be able to look for a point is a Compound Path or even a Path. This was a problem.

The first thought of solving this problem was to just make use of the Compound Path bounding box. The bounding box approach feels right until you
think about it for five seconds. A point in the
corner of a circle's bounding box is outside the
circle. You need something better.

This problem lead me to make some research which is how i found out about winding numbers.

## What are winding numbers?

Winding numbers count how many times a path's edges cross an imaginary ray cast from the point. The edge of a Path is made up of two consecutive points.

## How to calculate winding number

The first thing we have to do is to flatten the Path in order to get the points in the entire Path (This will be a discussion for another day). Now we can loop through each point. In each loop we get the current point and the next point after it to create our edge. Let's call this point p1 and p2.

```c++
 int winding = 0;
    auto points = path.flatten(0.5);

    for (size_t i = 0; i < points.size(); i++)
    {
        Vec2 p1 = points[i];
        Vec2 p2 = points[(i + 1) % points.size()];
    }
```

Now that we have the edge how can we check if the point passes through the edge?. We can do this by first creating a thin imaginary ray pointing at the right side of the point in the x direction. In order for this ray to pass there are two condition.

1. The y value of the point must be between p1 and p2. if both p1 and p2 are on the same side of the 
point's y level, the edge can't cross the ray — skip it.
2. The point.x must be at the left side of the edge (p1 -> p2). Remember that the ray is moving from the right side of point.x, so in order to pass through the edge it has to come from the left side of the edge (I actually had a hard time on this cause i was thinking the ray was facing upward).

Now with these two conditions met there's something else we need to consider. Whether the crossing is upward or downward determines whether we increment or decrement — upward crossings indicate a clockwise winding contribution, downward indicate counter-clockwise. This is important cause when the direction is ClockWise the winding number is increased (W++) while if it's Counter-ClockWise there's a decrease (W--). This is good in detecting if a point is inside a hole (outer path is clockwise while inner circle is counter-clockwise so they cancel each other out.)

`isLeft` computes the 2D cross product of the edge vector and the vector from p1 to the probe. A positive result means the probe is to the left of the edge direction — which means the crossX lands to the right of the probe, so the ray hits it. A negative result means the opposite — the ray misses.

```c++
 int CompoundPath::pathWindingNumber(const Path &path, Vec2 point)
{
    if (!path.isClosed() || path.pointCount() < 2)
        return 0;

    int winding = 0;
    auto points = path.flatten(0.5);

    for (size_t i = 0; i < points.size(); i++)
    {
        Vec2 p1 = points[i];
        Vec2 p2 = points[(i + 1) % points.size()];


        if (p1.y <= point.y)
        {
            // Upward Crossing (ClockWise)
            if (p2.y > point.y)
            {
                if (isLeft(p1, p2, point) > 0)
                {
                    winding++;
                }
            }
        }
        // Downward Crossing (Counter-ClockWise)
        else
        {
            if (p2.y <= point.y)
            {
                if (isLeft(p1, p2, point) < 0)
                {
                    winding--;
                }
            }
        }
    }
    return winding;
}
double CompoundPath::isLeft(Vec2 p1, Vec2 p2, Vec2 point)
{

    return (p2.x - p1.x) * (point.y - p1.y) - (point.x - p1.x) * (p2.y - p1.y);
}

```



So at the end of it all we compare the winding number to check if it's inside or outside the CompoundPath. For this we can check the FillType of the CompoundPath. There are two types `NonZero` and `EvenOdd`. Each of this type have a condition to check if the point is inside the compound path based on the winding number. For `NonZero`, just as the name says the winding number must not be equal to zero. In the case of `EvenOdd`, the winding number must not be an even number.

```c++
bool CompoundPath::contains(Vec2 point) const
{
    int windingCount = windingNumber(point);

    if (fillRule_ == FillRule::NonZero)
    {
        return windingCount != 0;
    }
    else
    {
        return (windingCount % 2) != 0;
    }
}

```

## What are the downside of winding number?

In terms of performance Winding number have an O(n) when n is the number of points in the path.
The solution to this will be to only check the winding number if necessary, for instance checking the bounding box if the point in inside and returning early can save time from finding the winding number
