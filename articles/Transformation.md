# Affine Transforms — How Graphics Engines Move Everything

I was testing my vector engine and added a rotation.The curve disappeared — not off by a bit, completely gone, rendering at x=-649. Here's what was happening and how affine transforms fix it permanently.

I finished building the `Paths`, `Compound Paths`, and even made some curve analysis method on the vector engine (`Arc Length`, `Curve-Curve Intersection`, `Path-Path Intersection` etc.). Everything started coming together but there was something that felt off. I couldn't make any sort of transformation to my curves (`translation`, `rotation`, `scale` etc.). I had to change the points of the curves, which is essentially rewriting the curve. So I made a few research and stumbled onto `Transformation matrix`.

## Transformation Matrix

```cpp
 // [a b tx]
 // [c d ty]
 // [0 0 1 ]
```

This is what is known as a `2D-affine transform matrix`. Each cell has a specific function:
a and d control how x and y scale.
b and c control rotation and skew — during a 90°
rotation, b becomes -sin and c becomes sin.
tx and ty simply shift the result after everything else.

This gives us a confined format to represent `Transformations` as a whole rather than each individual segment. You can even represent each Transformation type as it's own instance

```cpp

// {a,b,c,d,tx,ty}
 static Transform translate(double x, double y)
    {
        return {1, 0, 0, 1, x, y};
    }

    static Transform scale(double sx, double sy)
    {
        return {sx, 0, 0, sy, 0, 0};
    }

    static Transform rotate(double radians)
    {
        double cos = std::cos(radians);
        double sin = std::sin(radians);

        return {cos, -sin, sin, cos, 0, 0};
    }
    static Transform skewX(double radians)
    {
        return {1, std::tan(radians), 0, 1, 0, 0};
    }
    static Transform skewY(double radians)
    {
        return {1, 0, std::tan(radians), 1, 0, 0};
    }
```

This is not even the best part. What makes this form of Transform so strong is it's `Composition`. You can basically combine two or more `Transform` to basically create a new Transform with both properties

```cpp
 Transform operator*(const Transform &o) const
    {
        return {
            a * o.a + b * o.c,
            a * o.b + b * o.d,
            c * o.a + d * o.c,
            c * o.b + d * o.d,
            a * o.tx + b * o.ty + tx,
            c * o.tx + d * o.ty + ty,
        };
    }
```

With this you can perform `Transforms` like

```cpp
Transform::translate(100, 0) * Transform::rotate(1.57)

Transform::scale(2, 2) * Transform::translate(100,300)
```

After all this is said and done we still need to apply these Transforms to a point or a vector.

```cpp
 Vec2 apply(Vec2 p) const
    {
        return {
            a * p.x + b * p.y + tx,
            c * p.x + d * p.y + ty};
    }

    // Apply to vector (no translation)
    Vec2 applyToVector(Vec2 v) const
    {
        return {a * v.x + b * v.y, c * v.x + d * v.y};
    }
```

You might be wondering why do we need `applyToVector`, isn't it the same thing. Well, not quite. So while building the Path data structure section of the engine, I built a `PathPoint`. A PathPoint is basically made of the 3 main points, the handleIn, handleOut and position.
The `PathPoint` position is the main vector while the handleIn and handleOut are relative to the position. The reason for this was so that if theres a change in the position vector the handleIn and handleOut are also affected. This brings me back to why I created the `applyToVector`, so that I could apply all Transforms to that point except `translations` so as not the break the relative position that it has.

```cpp
void Path::transform(const Transform &t)
{
    for (auto &p : points_)
    {
        p.position = t.apply(p.position);
        p.handleIn = t.applyToVector(p.handleIn);
        p.handleOut = t.applyToVector(p.handleOut);
    }
}
```

## The Trap: Transforms Default to (0,0)

Rotating around the origin when your curve sits at
(300, 650) sends it sweeping through a massive arc
off screen. To fix this, you can first translate to the center of your curve and then apply the transform before going back to your initial shape origin (Learnt this the hard way while doing visualization 😅).

```cpp
Vec2 center = c.boundingBox().center();
Transform newT = Transform::translate(center.x, center.y) * t * Transform::translate(-center.x, -center.y);
```

## What's Next

Now my curves can now move without me changing their original points thanks to `Transforms`.

A transform matrix is just a compact way to say "scale, rotate, then move" — and composing them lets you build any transformation from three primitives.

The next thing on my mind to implement will be a `PathOffsetter` for creating strokes.

The source code for this can be found in this [repo](https://github.com/gideon-del/jsi-canvas/tree/main/cpp/vector-engine)

Thanks for reading. See you next time ✌️
