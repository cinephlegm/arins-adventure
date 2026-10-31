#pragma once
#include <iostream>
#include "Includes.h"

//TEMPlATE CODE!


class Collidable;
class CollidableRectangle;
class CollidablePolygon;

//base class for collidable objects
class Collidable {
public:
    std::string tag;

    Collidable(const std::string& tag) : tag(tag) {}

    virtual bool CheckCollision(const Rectangle& playerRect) = 0; // Pure virtual function for checking collision with a rectangle
    virtual ~Collidable() = default;
};

//class for collidable rectangles
class CollidableRectangle : public Collidable {
private:
    Rectangle rectangle;

public:
    CollidableRectangle(const Rectangle& rect, const std::string& tag = "")
        : Collidable(tag), rectangle(rect) {}

    bool CheckCollision(const Rectangle& playerRect) override {
        return CheckCollisionRecs(this->rectangle, playerRect);
    }

};

//class for collidable polygons
class CollidablePolygon : public Collidable {
private:
    std::vector<Vector2> polygonPoints;

    //There's a bug in CheckCollisionPointPoly RayLibV5.0. CustomCheckCollisionPointPoly() is the fixed code which is published here: https://github.com/raysan5/raylib/commit/ef92ced370815a47d6d417ce6394b3156dd4f528
    bool CustomCheckCollisionPointPoly(Vector2 point, Vector2* points, int pointCount)
    {
        bool inside = false;
        if (pointCount > 2)
        {
            for (int i = 0, j = pointCount - 1; i < pointCount; j = i++)
            {
                if ((points[i].y > point.y) != (points[j].y > point.y) &&
                    (point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
                {
                    inside = !inside;
                }
            }
        }
        return inside;
    }

    // Function to check collision between a rectangle and a polygon
    bool CheckCollisionRectPoly(Rectangle rect, std::vector<Vector2>& polygonPoints) {
        // Define the corners of the rectangle
        Vector2 corners[4] = {
            {rect.x, rect.y}, // Top-left
            {rect.x + rect.width, rect.y}, // Top-right
            {rect.x, rect.y + rect.height}, // Bottom-left
            {rect.x + rect.width, rect.y + rect.height} // Bottom-right
        };

        // Check if any corner is inside the polygon
        for (const auto& corner : corners) {
            if (CustomCheckCollisionPointPoly(corner, polygonPoints.data(), static_cast<int>(polygonPoints.size())))
            {
                return true; 
            }
        }

        // Check if any polygon vertex is within the rectangle
        for (const auto& point : polygonPoints) {
            if (CheckCollisionPointRec(point, rect)) {
                return true; 
            }
        }

        return false; 
    }

public:
    CollidablePolygon(const std::vector<Vector2>& points, const std::string& tag = "")
        : Collidable(tag), polygonPoints(points) {}

    bool CheckCollision(const Rectangle& playerRect)  override {
        return CheckCollisionRectPoly(playerRect, polygonPoints);
    }
};
