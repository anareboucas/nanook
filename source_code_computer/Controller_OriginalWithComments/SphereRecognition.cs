using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Collections;
using Microsoft.DirectX;

namespace Controller
{
    /**
     * @brief A sphere recognition.
     */
    class SphereRecognition
    {
        int maxRad = 0;
        int minRad = 0;
        float[, , ,] accumulation;
        //Returns a list of Sphere objects
        //Parameters:
        // RangeImage object
        // Array containing the edges of the image with an edge being a "1" and no-edge "0"
        //      THIS CAN BE NULL
        // Maximum Radius
        // Minimum Radius
        // Edge threshold (see detectEdges method)
        // Percentile, which percentaje of the possible spheres will be discarded (based on their score)

        /**
         * @brief Returns a list of Sphere objects
         * @param rangeData A RangeImage Object
         * @param edges A float[,] - Array containing the edges of the image with an edge being a "1" and no-edge "0". THIS CAN BE NULL.
         * @param maxRad An integer - Maximum Radius
         * @param minRad An integer - Minimum Radius
         * @param edgeThreshold A float - Edge threshold (see @sa detectEdges method)
         * @param percentile A float - Which percentage of the possible spheres will be discarded (based on their score)
         * @returns The found spheres
         */
        public List<Sphere> FindSpheres(RangeImage rangeData, float[,] edges, int maxRad, int minRad, float edgeThreshold, float percentile)
        {
            DebugTimer dt = new DebugTimer();

            dt.debug("Find spheres!");

            //Determine the range of radii that we will search for
            int diffRadii = maxRad - minRad;

            //Create the array that will hold the edge data
            //Normal bi-dimensional array x,y
            if (edges == null)
            {
                edges = new float[rangeData.Width + 1, rangeData.Height + 1];
                edges = detectEdges(rangeData, edgeThreshold);
                //edges = detectEdgesRestricted(spheres, float.Parse(edgeTop.Text), float.Parse(edgeBottom.Text));
            }

            //Initialize the array that will hold the accumulation data
            //First two dimensions are the are a x,y plane for the accumulation
            //Third dimension is the radius for which that accumulation is done
            //Forh dimension is only 2 items long:
            //      -The first one is the accumulation value
            //      -The second is an average of all the distances (z) to the point
            //      in the edge array that projects a circle over this particular
            //      x,y,k(radius) point
            accumulation = new float[rangeData.Width + 1, rangeData.Height + 1, diffRadii, 4];

            //Initialize an array for recording the local MAX and MIN from each accumulation for 
            //all the different radii. The first dimension of the array is the computed radius, 
            //and on the second dimension the item 0 is the minimum value, and the item 1 is the max.
            float[,] localMaxMin = new float[maxRad - minRad, 2];

            float tmpHeight = 0f;

            dt.debug("Starting Hough transform algorithm...");

            for (int i = 0; i < edges.GetLength(0); i++)
            {
                for (int j = 0; j < edges.GetLength(1); j++)
                {
                    //If we have an edge in this point on the edges array, we proceed
                    if (edges[i, j] == 1)
                    {
                        //For every radius
                        for (int k = minRad; k < maxRad; k++)
                        {
                            //statusText.Text = "Generating circles (x,y,r)("+i+","+j+","+k+")";
                            for (int a = 0; a < 360; a += 5)
                            {
                                //Circle with 'k' radius to be traced on layer 'k'

                                //Points on the circumference
                                int x = (int)(i + Math.Sin(a * (Math.PI / 180)) * k);
                                int y = (int)(j + Math.Cos(a * (Math.PI / 180)) * k);

                                //If we are withing the bounds of the image
                                if (x > 0 && x < edges.GetLength(0) && y > 0 && y < edges.GetLength(1))
                                {
                                    tmpHeight = accumulation[x, y, maxRad - k - 1, 0] + 1F;

                                    //ACCUMULATE!
                                    accumulation[x, y, maxRad - k - 1, 0] = tmpHeight;
                                    //Update the average of distances of the circumference
                                    accumulation[x, y, maxRad - k - 1, 1] += rangeData.GetCustom(i, j);
                                    //Update the closest point in the circumference
                                    if (rangeData.GetCustom(i, j) < accumulation[x, y, maxRad - k - 1, 2] || accumulation[x, y, maxRad - k - 1, 2] == 0)
                                        accumulation[x, y, maxRad - k - 1, 2] = rangeData.GetCustom(i, j);

                                    if (rangeData.GetCustom(i, j) > accumulation[x, y, maxRad - k - 1, 2])
                                        accumulation[x, y, maxRad - k - 1, 3] = rangeData.GetCustom(i, j);
                                    //accumulation[x, y, maxRad - k - 1, 1] = (accumulation[x, y, maxRad - k - 1, 1] + rangeData.GetCustom(i, j)) / 2;

                                    //If we have a local (in the "layer", for a specific radius) MINIMUM we record it
                                    if (tmpHeight < localMaxMin[maxRad - k - 1, 0] || localMaxMin[maxRad - k - 1, 0] == 0)
                                        localMaxMin[maxRad - k - 1, 0] = tmpHeight;
                                    //If we have a local (in the "layer", for a specific radius) MAXIMUM we record it
                                    if (tmpHeight > localMaxMin[maxRad - k - 1, 1])
                                        localMaxMin[maxRad - k - 1, 1] = tmpHeight;
                                }
                            }
                        }
                    }
                }
            }

            dt.debugEnlapsed("Done accumulating...");
            dt.debug("Starting tests...");

            //Global list of positions, radius, accumulatedValues (SCORE), the average distance from center to midpoints, and average to edge points of spheres
            List<Point> centers = new List<Point>();
            List<int> radii = new List<int>();
            List<float> accumulatedValues = new List<float>();
            List<double> midPoints = new List<double>();
            List<double> avgEdges = new List<double>();
            List<double> avgDistances = new List<double>();
            List<double> diffCircumferences = new List<double>();

            //Initialize variables
            float tmpPerc = 0;
            Point tmpCenter = new Point(0, 0);
            double avgDistance, distance, formula1, formula2, valN, valE, valS, valW, avgPoints, avgEdge, pointDifference, offset, diffCircumference;

            float pointDifferenteThreshold = 0.03f;//0.02
            float midPointDifferenteThreshold = 0.02f;//0.01

            //DEBUG
            List<String> trash = new List<String>();

            for (int k = 0; k < accumulation.GetLength(2); k++)
            {
                //Calculate half the radius for sphere double cheching in line 163-179
                offset = (maxRad - (float)k) / 2F;
                for (int i = 0; i < rangeData.Width - 1; i++)
                {
                    for (int j = 0; j < rangeData.Height - 1; j++)
                    {
                        //Calculate if the point is in the predefined percentile
                        tmpPerc = Math.Abs(accumulation[i, j, k, 0] - localMaxMin[k, 0]) / localMaxMin[k, 1];

                        //Average distance to the center of the sphere
                        avgDistance = (rangeData.GetCustom((i + 1 > rangeData.Width - 2) ? i : i + 1, j) + rangeData.GetCustom((i < 1) ? i + 1 : i - 1, j) + rangeData.GetCustom(i, (j + 1 > rangeData.Height - 2) ? j : j + 1) + rangeData.GetCustom(i, (j < 1) ? j + 1 : j - 1)) / 4f;

                        //Average edge distance
                        avgEdge = accumulation[i, j, k, 1] / accumulation[i, j, k, 0];

                        //Distance from center to edges 
                        //(difference between average distance at the circumference, minus the center)
                        distance = avgEdge - avgDistance;

                        //Formula that approximates de excpected distance difference for a sphere of radius k
                        //formula1 = (7f / 1800f) * (maxRad - k) + (122f / 900f);

                        //Threshold for difference between expected and optimal
                        //float thresholdDistance = 0.2f;

                        //IF the point is in the predefined percentile and 
                        //the distance from edges of the circumference and the center is possitive and 
                        //the difference between the average distance at circumference minus the formula for k is 
                        // bigger tan threshold
                        //if (tmpPerc > percentile / 100 && distance > 0 && (distance - formula1) < thresholdDistance)
                        //-Math.Min(0,(maxRad-k-15))

                        if (tmpPerc > (percentile / 100) || (tmpPerc >= (percentile / 100) - ((float)(maxRad - k) / 90)))
                        {
                            //Get distances of four points in the sphere surface
                            valE = rangeData.GetCustom((i - offset < 0) ? i : (int)(i - offset), j);
                            valS = rangeData.GetCustom((i + offset > rangeData.Width - 1) ? i : (int)(i + offset), j);
                            valW = rangeData.GetCustom(i, (j - offset < 0) ? j : (int)(j - offset));
                            valN = rangeData.GetCustom(i, (j + offset > rangeData.Height - 1) ? j : (int)(j + offset));
                            //Calculate the difference between the closest point, and the farther away
                            pointDifference = Math.Max(Math.Max(valE, valN), Math.Max(valS, valW)) - Math.Min(Math.Min(valE, valN), Math.Min(valS, valW));

                            //Average of all four points minus the distance to center
                            avgPoints = ((valE + valS + valW + valN) / 4);

                            //Formula for expected difference between center distance and distance of four points at k/2 for sphere with radius k
                            formula2 = ((17f / 9 * (float)(maxRad - k)) - 94f / 9f) / 1000f;

                            //Console.WriteLine("----------------\nSPHERE ID="+centers.Count+"\nradius="+(maxRad-k)+"\ndistance edges=" + accumulation[i, j, k, 1] + "\ndistanceAvg=" + avgDistance + "\n(distance edges)-distanceAvg=" + distance + "\n(Midpoint points avg distance)-avgDistance(center)=" + avgPoints + "\nNEWNUMBER***=" + formula2 + "\npointDifference=" + pointDifference+"\n--------------");

                            diffCircumference = accumulation[i, j, k, 3] - accumulation[i, j, k, 2];

                            //if (avgEdges > 1.6 && avgEdges < 1.9)

                            float diffToOptimal = Math.Abs(((float)(maxRad - k) / 800f) - (float)(avgPoints - avgDistance));

                            //if (pointDifference < 0.02d && avgPoints - formula2 > 0 && avgPoints - formula2 < 0.002d)
                            // && Math.Abs((avgPoints-avgDistance)-formula2)<0.1f

                            float tmptmp = (float)(avgEdge - (avgPoints + (float)(((float)(maxRad - k) * 2) / 100f)));
                            float tmptmptmp = (float)(maxRad - k) / 100;

                            double radiusMeters = (float)(maxRad - k) * (Math.Cos(89.75 * (Math.PI / 180)) * avgDistance);

                            float circumferenceThreshold = 0.1f;
                            float distanceCenterPointsThreshold = 0.005f;
                            //&& tmptmp <  tmptmptmp//&& tmptmp < tmptmptmp
                            if (diffCircumference < circumferenceThreshold &&
                                avgPoints - avgDistance > distanceCenterPointsThreshold &&
                                pointDifference < pointDifferenteThreshold &&
                                diffToOptimal < midPointDifferenteThreshold &&
                                avgEdge > avgPoints && avgPoints > avgDistance &&
                                avgEdge - avgDistance > radiusMeters &&
                                avgEdge < avgDistance * 3 //&&
                                //                                     Magic Number!
                                //avgEdge - avgDistance < radiusMeters * 3
                                )
                            {
                                //Console.WriteLine("----------------\nSPHERE ID=" + centers.Count + "\nradius=" + (maxRad - k) + "\ndistance edges=" + avgEdge + "\ndistanceAvg=" + avgDistance + "\n(distance edges)-distanceAvg=" + distance + "\n(Midpoint points avg distance)-avgDistance(center)=" + avgPoints + "\nNEWNUMBER***=" + formula2 + "\npointDifference=" + pointDifference + "\nSCORE=" + accumulation[i, j, k, 0] + "\n--------------");
                                //trash.Add("distance edges=" + accumulation[i, j, k, 1] + "\ndistanceAvg=" + avgDistance + "\ndiference=" + distance + "\ndiff2 - avgDistance=" + avgPoints + "\nNEWNUMBER***="+formula2+"\npointDifference=" + pointDifference+ "\nRESULT=" + (avgPoints - distance * Math.Sin(45 * (Math.PI / 180))));

                                //Add the values of this sphere to the global lists
                                centers.Add(new Point(i, j));
                                radii.Add(maxRad - k);
                                accumulatedValues.Add(accumulation[i, j, k, 0]);
                                midPoints.Add(avgPoints);
                                avgEdges.Add(avgEdge);
                                avgDistances.Add(avgDistance);
                                diffCircumferences.Add(diffCircumference);
                            }
                        }
                    }
                }
            }

            dt.debugEnlapsed("Done with tests...");


            List<Sphere> repeatedList = new List<Sphere>();
            double deg2rad = Math.PI / 180;

            double realRadius, theta, phi;

            for (int i = 0; i < centers.Count; i++)
            {
                Vector3 absolutePosition = new Vector3();

                //double theta = Math.Asin(((centers[i].X - rangeData.Width / 2) * Math.Cos(89.75 * deg2rad)) / avgDistances[i]);
                //double phi = Math.Asin(((centers[i].Y - rangeData.Height / 2) * Math.Cos(89.75 * deg2rad)) / avgDistances[i]);
                theta = Math.Asin((centers[i].X - rangeData.Width / 2) * Math.Cos(89.75 * deg2rad));
                phi = Math.Asin((centers[i].Y - rangeData.Height / 2) * Math.Cos(89.75 * deg2rad));

                absolutePosition.X = (float)(avgDistances[i] * Math.Cos(phi) * Math.Sin(theta));
                absolutePosition.Y = (float)(avgDistances[i] * Math.Sin(phi) * Math.Sin(theta));
                absolutePosition.Z = (float)(avgDistances[i] * Math.Cos(phi));

                realRadius = avgDistances[i] * Math.Cos(89.75 * deg2rad) * radii[i];

                Sphere a = new Sphere(absolutePosition, centers[i], radii[i], (float)realRadius, accumulatedValues[i], (float)avgEdges[i], (float)avgDistances[i], (float)midPoints[i], (float)diffCircumferences[i]);
                repeatedList.Add(a);
            }

            List<Sphere> finalList = new List<Sphere>();

            dt.debug("Start sphere merging algorithm...");

            //Minimum dstance to consider that two centers are the same sphere
            int minDistanceThreshold = 16;

            //finalList = repeatedList;
            /**/
            List<Sphere> toMerge = new List<Sphere>();

            IEnumerator<Sphere> enu = repeatedList.GetEnumerator();
            IEnumerator<Sphere> enu2;

            List<Sphere> tmpSpheres = new List<Sphere>(repeatedList);

            int limit = repeatedList.Count - 1;

            Sphere actual, test;

            while (enu.MoveNext())
            {
                actual = enu.Current;

                toMerge.Clear();
                toMerge.Add(actual);

                if (tmpSpheres.Contains(actual))
                {
                    enu2 = tmpSpheres.GetEnumerator();
                    while (enu2.MoveNext())
                    {
                        test = enu2.Current;
                        if (actual != test && pointDistance(actual.center, test.center) < minDistanceThreshold)
                            toMerge.Add(test);
                    }
                    foreach (Sphere sph in toMerge)
                        tmpSpheres.Remove(sph);

                    if (toMerge.Count > 1)
                        finalList.Add(mergeSpheres(toMerge));
                    else
                        finalList.Add(actual);
                }
            }

            dt.debugEnlapsed("Done with merging...");
            /**/

            List<Sphere> balancedFinalList = new List<Sphere>(finalList);

            /**/
            dt.debug("Finally filtering by score...");
            foreach (Sphere sph in finalList)
            {
                if (sph.score < sph.radius * Math.PI)
                    balancedFinalList.Remove(sph);
            }
            /**/
            dt.debugEnlapsed("Final list prepared.");

            dt.debug("Returning list of spheres (" + balancedFinalList.Count + "). Done.");
            //return repeatedList;
            return balancedFinalList;
        }

        //Detect the Edges!
        //Returns a bi-dimensional array sized as RangeImage width and height + 1
        //with an "1" for every pixel (x,y) where an edge is detected
        //Parameters
        // RangeImage object, double with threshold to determine "layers" in the picture
        //                   (How separated the object has to be from the background 
        //                    to be considered an object and not part of the background)

        /**
         * @brief Detect edges:
         * @param threshold A float
         * @param rangeData A RangeImage object - @brief Double with threshold to determine "layers" in the picture
         *                                        @brief (How separated the object has to be from the background
         *                                        @brief to be considered an object and not part of the background)
         * @returns edgesGen - A float[,]
         * @brief A bi-dimensional array sized as RangeImage width and height + 1
         */
        public float[,] detectEdges(RangeImage rangeData, float threshold)
        {

            int cont = 0;
            float[,] edgesGen = new float[rangeData.Width + 1, rangeData.Height + 1];
            for (int i = 2; i < rangeData.Width - 3; i++)
            {
                for (int j = 2; j < rangeData.Height - 3; j++)
                {
                    //Calculate neighboring pixels
                    float wZ = rangeData.GetCustom(i, j - 1);
                    float eZ = rangeData.GetCustom(i, j + 1);
                    float nZ = rangeData.GetCustom(i - 1, j);
                    float sZ = rangeData.GetCustom(i + 1, j);

                    double diffX = Math.Abs(wZ - eZ);
                    double diffY = Math.Abs(nZ - sZ);

                    //If we have a match and its not neighboring other edges, we mark it as edge
                    if ((diffX > threshold * rangeData.GetCustom(i, j) && edgesGen[i, j - 1] == 0 && edgesGen[i, j - 2] == 0) || (diffY > threshold * rangeData.GetCustom(i, j) && edgesGen[i - 1, j] == 0 && edgesGen[i - 2, j] == 0))
                    //if ((diffX > threshold && edgesGen[i, j - 1] == 0 && edgesGen[i, j - 2] == 0) || (diffY > threshold && edgesGen[i - 1, j] == 0 && edgesGen[i - 2, j] == 0))
                    //if ((diffX > threshold && edgesGen[i, j - 1] == 0) || (diffY > threshold && edgesGen[i - 1, j] == 0))
                    //if (diffX > threshold || diffY > threshold)
                    {
                        //Console.WriteLine("Edge at: ("+i+","+j+")");
                        cont++;
                        edgesGen[i, j] = 1;
                    }
                }

            }
            return edgesGen;
        }

        /**
         * @brief Merge spheres
         * @param toMerge A List<Sphere>
         * @returns toMerge[maxPos] - A Sphere
         */
        private Sphere mergeSpheres(List<Sphere> toMerge)
        {
            int maxPos = 0;
            float tmp = 0;

            for (int i = 0; i < toMerge.Count; i++)
            {
                tmp += toMerge[i].score;
            }

            float avgScore = tmp / toMerge.Count;

            for (int i = 0; i < toMerge.Count; i++)
            {//toMerge[i].radius <= toMerge[maxPos].radius &&
                if (i != maxPos && (toMerge[i].score > toMerge[maxPos].score || Math.Abs(toMerge[i].score - avgScore) < toMerge[i].radius / 5))
                {
                    maxPos = i;
                }

            }
            toMerge[maxPos].score *= toMerge.Count;

            return toMerge[maxPos];

            /**
            int maxPos = 0;
            float accumulatedScore = 0;
            for (int i = 0; i < toMerge.Count; i++)
            {
                if (     (toMerge[i].score == toMerge[maxPos].score && toMerge[i].radius < toMerge[maxPos].radius) ||
                         toMerge[i].score > toMerge[maxPos].score ||
                         i == maxPos
                    )
                    maxPos = i;

                accumulatedScore += toMerge[i].score;
            }
            toMerge[maxPos].score = accumulatedScore;
            return toMerge[maxPos];
            /**/
        }

        /**
         * @brief Point Distance
         * @param point1 A Point
         * @param point2 A Point
         * @returns A double
         */
        private double pointDistance(Point point1, Point point2)
        {
            int tBase = Math.Abs(point1.X - point2.X);
            int tHeight = Math.Abs(point1.Y - point2.Y);
            return Math.Sqrt(Math.Pow(tBase, 2) + Math.Pow(tHeight, 2));
        }

        /**
         * @brief Point Average
         * @param point1 A Point
         * @param point2 A Point
         * @returns A Point
         */
        private Point pointAverage(Point point1, Point point2)
        {
            int x = point1.X - (point1.X - point2.X);
            int y = point1.Y - (point1.Y - point2.Y);
            return new Point(x, y);
        }
    }
}

