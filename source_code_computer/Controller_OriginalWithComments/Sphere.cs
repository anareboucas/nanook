using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Microsoft.DirectX;

namespace Controller
{
    /**
     * @brief A sphere
     */
    class Sphere
    {
        public Vector3 absolutePosition;
        public Point center;
        public int radius;
        public float realRadius;
        public float score;
        public float avgEdges;
        public float avgDistance;
        public float midPoints;
        public float diffCircumference;

        /**
         * @brief Constructor
         * @param absolutePosition A Vector3 (The absolute position)
         * @param center A Point (The center)
         * @param radius A integer (The radius)
         * @param realRadius A float (The real radius)
         * @param score A float (The score)
         * @param avgEdges A float (The average edges)
         * @param avgDistance A float (The average distance)
         * @param midPoints A float (The middle points)
         * @param diffCircumference A float (The difference circumference)
        */
        public Sphere(Vector3 absolutePosition, Point center, int radius, float realRadius, float score, float avgEdges, float avgDistance, float midPoints, float diffCircumference)
        {
            this.absolutePosition = absolutePosition;
            this.center = center;
            this.radius = radius;
            this.realRadius = realRadius;
            this.score = score;
            this.avgEdges = avgEdges;
            this.avgDistance = avgDistance;
            this.midPoints = midPoints;
            this.diffCircumference = diffCircumference;
        }

        /**
         * @brief Constructor.
         * @param absolutePosition A Vector3 (The absolute position)
         * @param realRadius A float (The real radius.)
         */
        public Sphere(Vector3 absolutePosition, float realRadius)
        {
            this.absolutePosition = absolutePosition;
            this.realRadius = realRadius;
        }
    }
}
