using System;
using System.IO;
using System.Collections.Generic;


class PathFinder
{
    const int speed = 40;
    const double r2 = 1.42;
    static int radius = 0; //distance to maintain from obsacles
    const int wallRatio = 100; // used to randomly generate obstacle maps
    const int obstacleSize = 1;
    const double costScale = 1.50; //how much to scale up hcosts >= 1. higher values usually find paths more quickly but not always best path
    const double maxGrad = 0.6;
    const double changeDirectionFactor = 1.00; // >= 1
    const bool cutCorners = false;
    static int length, width;
    double metersPerCellX, metersPerCellY;
    static int startX = -1;
    static int startY = -1;
    static int endX = -1;
    static int endY = -1;
    static double orientation = -1.0;
    static bool usingFile;
    public bool validFile;
    public bool validStartEnd = false;
    List<coord> closed;
    minHeap heap;


    public PathFinder(string fileName)
    {
        usingFile = true;
        validFile = true;
        closed = new List<coord>();
        try
        {
            using (StreamReader SR = File.OpenText(fileName))
            {
                length = Convert.ToInt32(SR.ReadLine());
                width = Convert.ToInt32(SR.ReadLine());
                metersPerCellX = Convert.ToDouble(SR.ReadLine());
                metersPerCellY = Convert.ToDouble(SR.ReadLine());
                heap = new minHeap(length, width);
                for (int index = 0; index < length * width; index++)
                {
                    heap.coords[index / width][index % length] =
                    heap.coords[index / width][index % length].setHeight(Convert.ToDouble(SR.ReadLine()));
                }
                setGrads();
                setStartEnd();
                SR.Close();

            }
        }


        catch (IOException ex)
        {
            validFile = false;
            Console.WriteLine(ex.Message);
        }
    }

    public PathFinder(int sX, int sY, int eX, int eY, double orientationAngle,bool[,] obstacles)
    {
        usingFile = true;
        validFile = true;
        closed = new List<coord>();
        length = obstacles.GetLength(0);
        width = obstacles.GetLength(1);
        metersPerCellX = 0.20;
        metersPerCellY = 0.20;
        heap = new minHeap(length, width);
        for (int x = 0; x < length; x++)
        {
            for (int y = 0; y < width; y++)
            {
                if (obstacles[x,y])
                    heap.coords[x][y] = heap.coords[x][y].setChar('O');
            }
        }
        setStartEnd(sX, sY, eX, eY, orientationAngle);
    }



    public PathFinder(string fileName, int sX, int sY, int eX, int eY, double orientationAngle)
    {
        
        usingFile = true;
        validFile = true;
        closed = new List<coord>();
        try
        {
            using (StreamReader SR = File.OpenText(fileName))
            {
                length = Convert.ToInt32(SR.ReadLine());
                width = Convert.ToInt32(SR.ReadLine());
                metersPerCellX = Convert.ToDouble(SR.ReadLine());
                metersPerCellY = Convert.ToDouble(SR.ReadLine());
                heap = new minHeap(length, width);
                for (int index = 0; index < length * width; index++)
                {
                    heap.coords[index / width][index % length] = 
                    heap.coords[index / width][index % length].setHeight (Convert.ToDouble(SR.ReadLine()));
                }
                setGrads();
                setStartEnd(sX, sY, eX, eY, orientationAngle);
                SR.Close();
            }
        }


        catch (IOException ex)
        {
            Console.WriteLine(ex.Message);
            validFile = false;
        }

    }


    public PathFinder(int x, int y)
    {
        usingFile = false;
        closed = new List<coord>();
        length = x; width = y;
        metersPerCellX = metersPerCellY = 1;
        heap = new minHeap(length, width);
        makeGrid();
        Console.WriteLine("Made grid");
    }


    public void setRadius(int newRadius) { radius = newRadius; }

    public void setGrads()
    {
        double gradient;
        for (int y = 0; y < width; y++)
        {
            for (int x = 0; x < length; x++)
            {
                double h = heap.coords[x][y].height;
                if (x - 1 < 0 || x + 1 >= length || y - 1 < 0 || y + 1 >= width || h == 2e8) //out of bounds
                    gradient = 2e8;
                else
                {
                    //average of slopes in 4 directions
                    gradient = (Math.Abs(heap.coords[x - 1][y].height - h) + Math.Abs(heap.coords[x + 1][y].height - h) +
                            Math.Abs(heap.coords[x][y - 1].height - h) + Math.Abs(heap.coords[x][y + 1].height - h)) / 4;

                }
                coord c = heap.coords[x][y];
                c.setGrad(gradient);
                if (gradient > maxGrad)
                {
                    c.setChar('O');

                }
                else
                    c.setChar('.');
                heap.coords[x][y] = c;
            }
        }
    }

    public void setStartEnd()
    {
        int count = 0;
        Random r = new Random();
        do
        {
            if (count == 10000000)
                validStartEnd = false;
            startX = r.Next(length);
            startY = r.Next(width);
            endX = r.Next(length);
            endY = r.Next(width);
            count++;
        } while ((startX == endX && startY == endY) || !isSpace(startX, startY) 
            || !isSpace(endX, endY));

        heap.coords[startX][startY] = heap.coords[startX][startY].setChar('S');
        heap.coords[endX][endY] = heap.coords[endX][endY].setChar('F');
        heap.resetCosts();
        closed.Clear();
        validStartEnd = true;
        orientation = r.NextDouble() * 360;
    }


    bool isSpace(int x, int y)
    {
        if (x + radius >= length || y + radius >= width || x - radius < 0 || y - radius < 0)
        {
            return false; //too close to border
        }
        for (int j = y - radius; j <= y + radius; j++)
        {
            for (int i = x - radius; i <= x + radius; i++)
            {
                if (heap.coords[i][j].c == 'O') //obstacle
                {
                    return false;
                }
            }
        }
        return true;
    }


    int getAngle(int dx, int dy)
    {
        if (dx > 0 && dy == 0)
            return 0;
        if (dx > 0 && dy > 0)
            return 45;
        if (dx == 0 && dy > 0)
            return 90;
        if (dx < 0 && dy > 0)
            return 135;
        if (dx < 0 && dy == 0)
            return 180;
        if (dx < 0 && dy < 0)
            return 225;
        if (dx == 0 && dy < 0)
            return 270;
        if (dx > 0 && dy < 0)
            return 315;
        return -1; //bad values for dy, dx
    }

    public void setStartEnd(int sX, int sY, int eX, int eY, double angle)
    {
        if (sX < 0 || sX >= length || sY < 0 || sY >= width || eX < 0 || eX >= length || eY < 0 || eY >= width ||
            (sX == eX && sY == eY))
        {
            Console.WriteLine("Invalid start and end coordinates");
            validStartEnd = false;
            return;
        }

        if (!isSpace(sX, sY) || !isSpace(eX, eY))
        {
            Console.WriteLine("Invalid start and end coordinates");
            validStartEnd = false;
            return;
        }

        if (startX >= 0 && startY >= 0 && startX < length && startX < width &&
            endX >= 0 && endY >= 0 && endX < length && endX < width)
        {
            heap.coords[startX][startY] = heap.coords[startX][startY].setChar ('.');
            heap.coords[endX][endY] = heap.coords[endX][endY].setChar('.');
        }

        startX = sX; startY = sY; endX = eX; endY = eY;
        heap.coords[startX][startY] = heap.coords[startX][startY].setChar('S');
        heap.coords[endX][endY] = heap.coords[endX][endY].setChar ('F');
        while (angle < 0)
            angle += 360;
        while (angle > 360)
            angle -= 360;
        orientation = angle;
        heap.resetCosts();
        closed.Clear();
        validStartEnd = true;
    }





    void setCost(coord parent, int x, int y)
    {
        if (!isSpace(x, y)) return;

        if (!cutCorners && (heap.coords[x][parent.y].c == 'O' || heap.coords[parent.x][y].c == 'O')) return;

        double grad = heap.coords[x][y].grad;

        double newGCost; //use gradient at (x,y) to determine cost

        if (x == parent.x || y == parent.y)
            newGCost = parent.gcost + 1;
        else
            newGCost = parent.gcost + (r2);

        /*
        if (x==parent.x || y==parent.y)
            newGCost=parent.gcost + (pow(2.,grad));
        else 
            newGCost = parent.gcost + (r2 * pow(2.,grad));
            */

        //change in direction scales up cost--to avoid extraneous turns
        /*if ((x - parent.x != parent.x - heap.coords[parent.parentX][parent.parentY].x ||
            y - parent.y != parent.y - heap.coords[parent.parentX][parent.parentY].y) &&
            !parent.equals(new coord(startX, startY, 0)))
            newGCost *= changeDirectionFactor;
         */

        //mistakenly marked the coord as known due to hueristic
        if (heap.coords[x][y].isKnown && heap.coords[x][y].gcost > newGCost)
        {
            heap.coords[x][y] = heap.coords[x][y].makeUnknown();
            heap.coords[x][y] = heap.coords[x][y].setGCost(newGCost);
            heap.coords[x][y] = heap.coords[x][y].setParent(parent.x, parent.y);
            heap.add(heap.coords[x][y]);
        }

        else if (heap.coords[x][y].isKnown) return;

        //never seen the coord before, so add it
        if (!heap.coords[x][y].isOpen())
        {
            heap.coords[x][y] = heap.coords[x][y].setGCost(newGCost);
            heap.coords[x][y] = heap.coords[x][y].setParent(parent.x, parent.y);
            heap.add(heap.coords[x][y]);
        }

        //found a better path to the coord
        else if (heap.coords[x][y].gcost > newGCost)
        {
            heap.removeIndex(heap.coords[x][y].heapIndex);
            heap.coords[x][y] = heap.coords[x][y].setGCost(newGCost);
            heap.coords[x][y] = heap.coords[x][y].setParent(parent.x, parent.y);
            heap.add(heap.coords[x][y]);
        }


    }


    void makeGrid() //make a random grid
    {
        Random r = new Random();

        for (int y = 0; y < width; y++)
        {
            for (int x = 0; x < length; x++)
            {
                int value = r.Next(wallRatio);
                if (value == 0)
                {
                    for (int i = Math.Max(0, x - (obstacleSize / 2)); i <= Math.Min(length - 1, x + ((obstacleSize - 1) / 2)); i++)
                    {
                        for (int j = Math.Max(0, y - (obstacleSize / 2)); j <= Math.Min(width - 1, y + ((obstacleSize - 1) / 2)); j++)
                        {
                            heap.coords[i][j] = heap.coords[i][j].setChar('O');
                        }
                    }
                }
                else if (heap.coords[x][y].c != 'O')
                    heap.coords[x][y] = heap.coords[x][y].setChar('.');

            }
        }

        do
        {
            startX = r.Next(length);
            startY = r.Next(width);
            endX = r.Next(length);
            endY = r.Next(width);
        }
        while (!isSpace(startX, startY) || !isSpace(endX, endY) || (startX == endX && startY == endY));
        heap.coords[startX][startY] = heap.coords[startX][startY].setChar('S');
        heap.coords[endX][endY] = heap.coords[endX][endY].setChar('F');
    }


    public void solve()
    {
        if (!validStartEnd) return;
        heap.setHCosts();
        coord end = new coord(endX, endY, 0);
        heap.coords[startX][startY] = heap.coords[startX][startY].setParent(startX, startY);
        heap.coords[startX][startY] = heap.coords[startX][startY].setGCost(0);
        heap.add(heap.coords[startX][startY]);
        coord current;// = new coord(-1, -1, 0);
        while (!heap.empty())
        {
            current = heap.removeMin();

            if (current.equals(end)) break;
            heap.coords[current.x][current.y] = heap.coords[current.x][current.y].makeKnown();
            closed.Add(current);
            setCost(current, current.x - 1, current.y);
            setCost(current, current.x + 1, current.y);
            setCost(current, current.x, current.y - 1);
            setCost(current, current.x, current.y + 1);
            setCost(current, current.x - 1, current.y - 1);
            setCost(current, current.x + 1, current.y - 1);
            setCost(current, current.x - 1, current.y + 1);
            setCost(current, current.x + 1, current.y + 1);
        }
        Console.WriteLine("Open: " + heap.size);
        Console.WriteLine("Closed: " + closed.Count);

    }

    public void printGrid()
    {
        try
        {
            FileStream gridFStream = File.Open("grid.txt", FileMode.Create);
            if (gridFStream.CanWrite)
            {
                for (int j = 0; j < width; j++)
                {
                    for (int i = 0; i < length; i++)
                    {
                        gridFStream.WriteByte((byte)heap.coords[i][j].c);
                    }
                    gridFStream.WriteByte((byte)'\n');
                }
                //gridFStream.Close();
            }
        }

        catch (IOException ex)
        {
            Console.WriteLine(ex.Message);
        }
    }



    public List <String> getPath()
    {
        List<coord> coordList = new List<coord>();
        int x, y;
        if (closed.Count == 0) return null;
        coord lowest = closed[0];
        if (heap.coords[endX][endY].cost >= 0) //found a path
        {
            x = heap.coords[endX][endY].parentX;
            y = heap.coords[endX][endY].parentY;
            coordList.Add(heap.coords[endX][endY]);
        }
        else
        {
            
            for (int index = 1; index < closed.Count; index++)
            {
                coord c = closed[index];
                if (c.cost >= 0 && c.hcost < lowest.hcost)
                {
                    lowest = c;
                }
            }

            x = lowest.x;
            y = lowest.y;
        }
        
        while (heap.coords[x][y].c != 'S')
        //retrace steps back to the start
        {
            heap.coords[x][y] = heap.coords[x][y].setChar('*'); //path space
            coordList.Add(heap.coords[x][y]);
            int newX = heap.coords[x][y].parentX;
            int newY = heap.coords[x][y].parentY;
            x = newX;
            y = newY;
        }

        coordList.Add(heap.coords[startX][startY]);

        // send the path coordinates to file "coords.txt"
        try
        {
            FileStream coordsFStream = File.Open("coords.txt", FileMode.Create);
            if (coordsFStream.CanWrite)
            {
                for (int i = coordList.Count - 1; i >= 0; i--)
                {
                    string coordString = coordList[i].x + ", " + coordList[i].y;
                    for (int j = 0; j < coordString.Length; j++)
                    {
                        coordsFStream.WriteByte((byte)coordString[j]);
                    }
                    coordsFStream.WriteByte((byte)'\n');
                }
                coordsFStream.Close();
            }
        }

        catch (IOException ex)
        {
            Console.WriteLine(ex.Message);
        }



        List<string> moveList = new List<string>();
        

        if (heap.coords[endX][endY].cost >= 0)
        {
            x = endX; y = endY;
        }
        else
        {
            x = lowest.x;
            y = lowest.y;
        }

        int parentX = heap.coords[x][y].parentX,
            parentY = heap.coords[x][y].parentY;
        int dx = parentX - x, dy = parentY - y, newDX, newDY;
        while (true)
        {
            if (heap.coords[x][y].c == 'S') break;
            int spaces;
            //count how many spaces in direction you move
            for (spaces = 0; heap.coords[x][y].parentX == x + dx && heap.coords[x][y].parentY == y + dy; spaces++)
            {
                x += dx;
                y += dy;
            }

            double meters = spaces * metersPerCellX; //actual distance moved
            while (meters > 0)
            {
                double tempMove = Math.Min(meters, 99.0);
                string spaces_str = "forward " + speed + " " + (tempMove * 100);
                moveList.Insert(0, spaces_str);
                meters -= 99.0;
            }

            if (heap.coords[x][y].c == 'S') break;

            newDX = heap.coords[x][y].parentX - x;
            newDY = heap.coords[x][y].parentY - y;

            //find turn angle
            int angle = getAngle(dx, dy);
            int newAngle = getAngle(newDX, newDY);
            int angleChange = angle - newAngle;
            string angle_str = "";
            if (angleChange < 0)
                angleChange += 360;
            if (angleChange <= 180)
                angle_str = "right " + speed + " ";
            else
            {
                angleChange = 360 - angleChange;
                angle_str = "left " + speed + " ";
            }

            angle_str += angleChange;
            moveList.Insert(0, angle_str);
            parentX = heap.coords[x][y].parentX;
            parentY = heap.coords[x][y].parentY;
            dx = newDX; dy = newDY;
        }

        //we're back at the start location
        //find the original orientation angle
        double ang = getAngle(-dx, dy);
        double changeAng = ang - orientation;
        if (changeAng > 180)
            changeAng -= 360;
        if (changeAng > 0)
            moveList.Insert(0, "left " + speed + " " + changeAng);
        else
            moveList.Insert(0, "right " + speed + " " + (-1 * changeAng));
        moveList.Insert(moveList.Count, "stop");

        try
        {
            FileStream movesFStream = File.Open("moves.txt", FileMode.Create);
            if (movesFStream.CanWrite)
            {
                for (int i = 0; i < moveList.Count; i++)
                {
                    for (int j = 0; j < moveList[i].Length; j++)
                    {
                        movesFStream.WriteByte((byte)moveList[i][j]);
                    }
                    movesFStream.WriteByte((byte)'\n');
                }
                movesFStream.Close();
            }
        }

        catch (IOException ex)
        {
            Console.WriteLine(ex.Message);
        }

        return moveList;
    }



    struct coord
    {
        public int x;
        public int y;
        public int heapIndex;
        public double cost; //total cost
        public double gcost;
        public double hcost;
        public bool isKnown;
        public int parentX;
        public int parentY;
        public double height;
        public double grad;
        public char c; //signifies type of cell

        public coord(int xVal, int yVal, double gradient)
        {
            x = xVal; y = yVal; cost = -1; gcost = -1; hcost = -1;
            parentX = parentY = -1;
            heapIndex = -1;
            height = 0;
            c = '.';
            isKnown = false;
            if (usingFile)
            {
                grad = gradient;
                if (gradient == 2e8)
                    c = 'O';
            }

            else
                grad = 0;
        }

        public coord setChar(char val) { c = val; return this; }

        public coord setHeapIndex(int HI) { heapIndex = HI; return this; }

        public coord setCost(double newCost) { cost = newCost; return this; }

        public coord setGrad(double gradient) { grad = gradient; return this; }

        public coord setHeight(double h) { height = h; return this; }

        public coord makeKnown()
        {
            isKnown = true;
            return this;
        }

        public coord makeUnknown()
        {
            isKnown = false;
            return this;
        }

        public coord setGCost(double newGCost)
        {
            gcost = newGCost;
            cost = gcost + hcost;
            return this;
        }

        public bool isOpen() { return cost >= 0; }


        public double compareTo(coord other)
        {
            return cost - other.cost;
        }

        public bool equals(coord other)
        {
            if (x == other.x && y == other.y)
                return true;
            return false;
        }
        public coord setParent(int xVal, int yVal)
        {
            parentX = xVal; parentY = yVal;
            return this;
        }

        public coord diagonalDistance()
        {
            int dx = Math.Abs(x - endX);
            int dy = Math.Abs(y - endY);
            hcost = costScale * ((Math.Min(dx, dy) * r2) + Math.Abs(dy - dx));
            return this;
        }
    };

    //holds coord heap in form of array and 2d array of coords, with heap indicies
    struct minHeap
    {
        public int size;
        List<coord> theHeap;
        public List<List<coord>> coords;

        public minHeap(int hor, int ver)
        {
            size = 0;
            theHeap = new List<coord>(length * width + 1);
            theHeap.Insert(0, new coord(-1, -1, 0));
            coords = new List<List<coord>>(length);
            for (int i = 0; i < coords.Capacity; i++)
            {
                coords.Insert(i, new List<coord>(width));
            }
            for (int y = 0; y < width; y++)
            {
                for (int x = 0; x < length; x++)
                {
                    coords[x].Insert(y, new coord(x, y, 0));
                }
            }
        }



        public void resetCosts()
        {
            for (int j = 0; j < width; j++)
            {
                for (int i = 0; i < length; i++)
                {
                    double grad = coords[i][j].grad;
                    char val = '.';
                    if (coords[i][j].c == 'O')
                        val = 'O';
                    if (i == startX && j == startY)
                        val = 'S';
                    if (i == endX && j == endY)
                        val = 'F';
                    coords[i][j] = new coord(i, j, grad);
                    coords[i][j] = coords[i][j].setChar(val);
                }
            }
            clearHeap();
        }

        public void clearHeap()
        {
            size = 0;
        }

        public void setHCosts()
        {
            for (int y = 0; y < width; y++)
            {
                for (int x = 0; x < length; x++)
                {
                    coords[x][y] = coords[x][y].diagonalDistance();
                }
            }
        }

        public bool empty() { return size == 0; }

        void swap(int index1, int index2)
        {

            coord temp1 = theHeap[index1];
            coord temp2 = theHeap[index2];
            int x1 = theHeap[index1].x;
            int y1 = theHeap[index1].y;
            int x2 = theHeap[index2].x;
            int y2 = theHeap[index2].y;
            temp2.setHeapIndex(index1);
            temp1.setHeapIndex(index2);
            theHeap[index1] = temp2;
            theHeap[index2] = temp1;
            coords[x1][y1] = temp1;
            coords[x2][y2] = temp2;
        }

        public void add(coord c)
        {
            size++;
            c.setHeapIndex(size);
            theHeap.Insert(size, c);
            coords[c.x][c.y] = c;
            int index = size;
            while (true)
            {
                coord current = theHeap[index];
                if (index / 2 < 1) break; // index is the head of the heap
                coord parent = theHeap[index / 2];
                if (current.compareTo(parent) > 0) break;
                swap(index / 2, index); //swap with parent
                index /= 2;
            }
        }

        public coord removeMin()
        {
            return removeIndex(1);
        }

        public coord removeIndex(int index)
        {
            coord removeMe = theHeap[index];
            removeMe.setHeapIndex(-1);

            coords[removeMe.x][removeMe.y] = removeMe;

            swap(index, size);
            size--;
            while (true)
            {
                coord current = theHeap[index];
                if (2 * index > size)
                {
                    return removeMe; //no children
                }
                coord left = theHeap[2 * index];
                if (2 * index == size) //only has left child
                {
                    if (left.compareTo(current) < 0)
                        swap(index, 2 * index);
                    return removeMe;
                }
                coord right = theHeap[2 * index + 1];
                if (left.compareTo(current) < 0 &&
                    left.compareTo(right) < 0)
                {
                    swap(index, 2 * index);
                    index *= 2;
                }
                else if (right.compareTo(current) < 0)
                {
                    swap(index, 2 * index + 1);
                    index *= 2; index++;
                }

                else
                {
                    return removeMe;
                }
            }
        }
    };

}