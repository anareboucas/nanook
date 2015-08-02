
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Collections;
using Microsoft.DirectX;
using System.IO;


namespace Controller
{

    class SphereRecognition2
    {
        const int VERTICAL_RESOLUTION = 360;
        const int HORIZONTAL_RESOLUTION = 401;

	    int groupNum = 0;
        double subsTol = 0;
        int numP = 0; //Number of points
        Vector3[] spCr = new Vector3[100]; // Sphere centers
        int[] spBl = new int[100]; //Sphere center obtained from clusters
        Vector3[] pointLine = new Vector3[VERTICAL_RESOLUTION * HORIZONTAL_RESOLUTION];
        Vector3[,] points = new Vector3[VERTICAL_RESOLUTION, HORIZONTAL_RESOLUTION];
        int[,] spPt = new int[VERTICAL_RESOLUTION, HORIZONTAL_RESOLUTION];
        int[,] gpNm = new int[VERTICAL_RESOLUTION, HORIZONTAL_RESOLUTION];


        public List<Sphere> FindSpheres(RangeImage rangeData)
        {

            int maxLp = 0;

            List<Sphere> sphereList = new List<Sphere>();
            
            do {

                dataProcess(rangeData);

                subsTol = subsTol + .05;
                maxLp++;
            } while (groupNum <= 6 && maxLp < 9);

            for (int i = 1; i < groupNum; i++)
            {
                if (spBl[i] == 1)
                {
                    Sphere updtSphere = new Sphere(spCr[i],(float).11777);
                    sphereList.Add(updtSphere);
                }

            }

            return sphereList;
        }


        private void dataProcess(RangeImage rangeData)
        {
            int inc = 0;
            numP = 0;
            groupNum = 0;

            TextWriter nw = new StreamWriter("spTesting.txt");
            //nw.WriteLine(subsTol);

            //Load data into a matrixv and initialize variables

            Vector3[] pointLine = rangeData.ConvertToPoints(float.MaxValue);

            for (int i = 0; i<VERTICAL_RESOLUTION; i++) {
                for (int j = 0; j<HORIZONTAL_RESOLUTION; j++) {
                    points[i,j] = pointLine[inc];
                    inc++;

                    //nw.WriteLine(pointLine[inc].X);
                    //nw.WriteLine(pointLine[inc].Y);
                    //nw.WriteLine(pointLine[inc].Z);

                    gpNm[i,j] = 0; //Initializes the group number for clustering
			        if (Math.Sqrt(Math.Pow(points[i,j].X,2)+Math.Pow(points[i,j].Y,2)+Math.Pow(points[i,j].Z,2)) < 30.0) {
				        spPt[i,j] = 0; //Assign the value 0 to all the points (for sphere detection)
			        } else {
				        spPt[i,j] = 5;
			        }

                }
            }

            //preProcess data to filter points that can be potential spheres

            int iPr = 0;
	        int jPr = 0;

	        for (int i=0;i<VERTICAL_RESOLUTION;i++) {
		        for (int j=1;j<HORIZONTAL_RESOLUTION;j++) {
			        if (spPt[i,j] != 5) { 
				        if (iPr != i) {
					        iPr = i;
					        jPr = 0;
				        }
				        if (Math.Sqrt(Math.Pow(points[i,j].X-points[iPr,jPr].X,2)+Math.Pow(points[i,j].Y-points[iPr,jPr].Y,2)+Math.Pow(points[i,j].Z-points[iPr,jPr].Z,2)) > 2.0*.117775 || jPr == 0) {			
					        spPt[i,j] = 1;
					        spPt[iPr,jPr] = 1;

                            //nw.WriteLine(points[i,j].X);
                            //nw.WriteLine(points[i,j].Y);
                            //nw.WriteLine(points[i,j].Z);
				        }
				        if (Math.Sqrt(Math.Pow(points[i,j].X-points[i,j-1].X,2)+Math.Pow(points[i,j].Y-points[i,j-1].Y,2)+Math.Pow(points[i,j].Z-points[i,j-1].Z,2)) > 2.0*.117775) {
					        iPr = i;
					        jPr = j;
				        } 
				        if (j==400 || j==401 || j==0 || j== 1) {
					        spPt[i,j] = 1;
				        }
			        }
		        } // End of "j loop"
	        } //End of "i loop"


            //Process data and find points that belong to spheres. Use the sphere equation to process each point and its neighbors

	        double rad = .11777;
	        double mag = 0.0;
	        double[] nrm = {0.0,0.0,1.0};
	        int sum = 0;
	        int cond = 0;
	        int maxSum = 0;
	        int iMax = 0;
	        int jMax = 0;
	        double[] center=new double[3];
	        double tol = .001;
	        double z = 0.0;
	        double ptAng; //Angle used to obtain resolution
	        int ptRes; //Point resolution
            double newPtRes;
            int newim;
            int newjn;
            
        	
	        for (int i=0;i<VERTICAL_RESOLUTION;i++) {
		        for (int j=10;j<HORIZONTAL_RESOLUTION;j++) {
			        sum = 0;
			        cond = 0;

			        mag = Math.Sqrt(Math.Pow(points[i,j].X,2)+Math.Pow(points[i,j].Y,2)+Math.Pow(points[i,j].Z,2));
			        nrm[0] = points[i,j].X/mag;
			        nrm[1] = points[i,j].Y/mag;
			        nrm[2] = points[i,j].Z/mag;

                    //nw.WriteLine(nrm[0]);
                    //nw.WriteLine(nrm[1]);
                    //nw.WriteLine(nrm[2]);

			        if (spPt[i,j] == 0) {
				        center[0] = points[i,j].X + rad*nrm[0];
				        center[1] = points[i,j].Y + rad*nrm[1];
				        center[2] = points[i,j].Z + rad*nrm[2];

                        //nw.WriteLine(center[0]);
                        //nw.WriteLine(center[1]);
                        //nw.WriteLine(center[2]);

				        //Obtaining point resolution based on distance to point
				        ptAng = Math.Asin(.09/Math.Sqrt(Math.Pow(points[i,j].X,2)+Math.Pow(points[i,j].Y,2)+Math.Pow(points[i,j].Z,2)));
				        ptRes = (int)Math.Floor(ptAng/.0043633);

                        //nw.WriteLine(ptAng);
                        //nw.WriteLine(ptRes);

				        for (int m=-ptRes;m<ptRes;m++) {
					        for (int n=-ptRes;n<ptRes;n++) {
                                newim = i + m;
                                newjn = j + n;
                                if (newim < 0) { newim = 0; }
                                if (newjn < 0) { newjn = 0; }
                                if (newim > 359) { newim = 359; }
                                if (newjn > 399) { newjn = 399; }

						        if (Math.Sqrt(Math.Pow(points[newim,newjn].X-points[i,j].X,2)+Math.Pow(points[newim,newjn].Y-points[i,j].Y,2)+Math.Pow(points[newim,newjn].Z-points[i,j].Z,2)) < rad) {
							        if (Math.Pow(center[2]-points[newim,newjn].Z,2)+Math.Pow(points[newim,newjn].X-center[0],2)+Math.Pow(points[newim,newjn].Y-center[1],2)-Math.Pow(rad,2) < tol) {
								        sum++; 
							        }
						        } 

					        }//End of 'n loop'
				        }//End of 'm loop'
        		
				        newPtRes = Math.Pow(2.0*(double)ptRes,2) - subsTol*Math.Pow(2.0*(double)ptRes,2);

				        if (newPtRes > 2.0) {

                            //nw.WriteLine(sum);

					        if ((double)sum > newPtRes && Math.Sqrt(Math.Pow(points[i,j].X,2)+Math.Pow(points[i,j].Y,2)+Math.Pow(points[i,j].Z,2)) < 7) {
						        spPt[i,j] = 3;
 
                                //nw.WriteLine(points[i,j].X);
                                //nw.WriteLine(points[i,j].Y);
                                //nw.WriteLine(points[i,j].Z);

					        }
				        }

			        }
		        } //End of 'i loop'
	        } //End of 'j loop'


            //Code to postprocess data

            groupNum = 1;
            int mMin = 0;
            int nMin = 0;
            double dis;
            double minDis = 10000.0;

            for (int i = 0; i < VERTICAL_RESOLUTION; i++)
            {
                for (int j = 0; j < HORIZONTAL_RESOLUTION; j++)
                {
                    if (spPt[i,j] == 3)
                    {
                        for (int m = 0; m < VERTICAL_RESOLUTION; m++)
                        {
                            for (int n = 0; n < HORIZONTAL_RESOLUTION; n++)
                            {
                                if (spPt[m,n] == 3)
                                {
                                    dis = Math.Sqrt(Math.Pow(points[i,j].X - points[m,n].X, 2) + Math.Pow(points[i,j].Y - points[m,n].Y, 2) + Math.Pow(points[i,j].Z - points[m,n].Z, 2));
                                    //Look at distance between points

                                    if (dis < .117775 && dis > 0)
                                    {
                                        if (gpNm[i,j] > 0)
                                        {
                                            gpNm[m,n] = gpNm[i,j];
                                        }
                                        if (gpNm[m,n] > 0)
                                        {
                                            gpNm[i,j] = gpNm[m,n];
                                        }
                                        if (gpNm[i,j] == 0 && gpNm[m,n] == 0)
                                        {
                                            gpNm[i,j] = groupNum;
                                            gpNm[m,n] = groupNum;
                                            groupNum++;
                                        }

                                    }

                                }
                            }//End of "m loop"
                        } //End of "n loop"

                    }
                } //End of "j loop"
            }//End of "i loop"

            //Assign group number to single points that are not part of a cluster

            for (int i = 0; i < VERTICAL_RESOLUTION; i++)
            {
                for (int j = 0; j < HORIZONTAL_RESOLUTION; j++)
                {
                    if (spPt[i,j] == 3 && gpNm[i,j] == 0)
                    {
                        gpNm[i,j] = groupNum;
                        groupNum++;
                    }
                }
            }


            //Find the centers of the clusters
           // TextWriter nw = new StreamWriter("spTesting.txt");


	        double[] nrm2 = new double[3];
	        double mag2 = 0;
	        //double rad = .11;

	        double[] sum2 = new double[100];

	        for (int i=0;i<100;i++) {
		        spCr[i].X = 0;
		        spCr[i].Y = 0;
		        spCr[i].Z = 0;
		        sum2[i] = 0;
	        }

	        for (int i=0;i<VERTICAL_RESOLUTION;i++) {
		        for (int j=0;j<HORIZONTAL_RESOLUTION;j++) {
			        if (gpNm[i,j] > 0) {
				        spCr[gpNm[i,j]].X = (spCr[gpNm[i,j]].X + points[i,j].X);
				        spCr[gpNm[i,j]].Y = (spCr[gpNm[i,j]].Y + points[i,j].Y);
				        spCr[gpNm[i,j]].Z = (spCr[gpNm[i,j]].Z + points[i,j].Z);
				        sum2[gpNm[i,j]] = sum2[gpNm[i,j]] + 1;
			        } 
		        }
	        }

            //Obtain average center of point clusters
	        for (int i=0;i<groupNum;i++) {
		        spCr[i].X = spCr[i].X/(float)(sum2[i]);
		        spCr[i].Y = spCr[i].Y/(float)(sum2[i]);
		        spCr[i].Z = spCr[i].Z/(float)(sum2[i]);

		        mag2 = Math.Sqrt(Math.Pow(spCr[i].X,2)+Math.Pow(spCr[i].Y,2)+Math.Pow(spCr[i].Z,2));
		        nrm2[0] = spCr[i].X/mag2;
		        nrm2[1] = spCr[i].Y/mag2;
		        nrm2[2] = spCr[i].Z/mag2;

		        spCr[i].X = spCr[i].X + (float)(rad*nrm2[0]);
		        spCr[i].Y = spCr[i].Y + (float)(rad*nrm2[1]);
		        spCr[i].Z = spCr[i].Z + (float)(rad*nrm2[2]);

                nw.WriteLine(spCr[i].X);
                nw.WriteLine(spCr[i].Y);
                nw.WriteLine(spCr[i].Z);

	        }

            nw.Close();

            //Obtain the centers of the spheres
            dis = 0;

	        for (int i=0;i<100;i++) {
		        spBl[i] = 0;
	        }

	        for (int i=0;i<groupNum;i++) {
		        for (int j=0;j<groupNum;j++) {
			        dis = Math.Sqrt(Math.Pow(spCr[i].X-spCr[j].Y,2)+Math.Pow(spCr[i].Y-spCr[j].Y,2)+Math.Pow(spCr[i].Z-spCr[j].Z,2));
			        if (dis > .35 && dis < .55) {			
				        spBl[i] = 1;
				        spBl[j] = 1;
			        } 
		        }
	        }


        }
    }
}
