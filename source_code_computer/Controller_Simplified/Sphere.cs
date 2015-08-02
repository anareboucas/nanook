using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Microsoft.DirectX;

namespace Controller
{
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

        public Sphere(Vector3 absolutePosition, float realRadius)
        {
            this.absolutePosition = absolutePosition;
            this.realRadius = realRadius;
        }
    }
}
