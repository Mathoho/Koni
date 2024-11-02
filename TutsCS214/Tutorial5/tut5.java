import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

public class tut5 {
   ArrayList<Point> points = new ArrayList<Point>();
   

    public class Point {
        int xc;
        int yc;
        boolean vl;

        public Point(int x, int y, boolean val) {
            this.xc = x;
            this.yc = y;
            this.vl = val;
        }

    }

    public tut5() {
        
    }

    public void makeBoard(int height, int width, int cellsize) {
        StdDraw.setCanvasSize(600, 600);
        StdDraw.setXscale(0.0, width + 5);
        StdDraw.setYscale(0.0, height + 5);
        StdDraw.setPenRadius(0.025);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
            StdDraw.point(i + cellsize, j + cellsize);
          Point fir = new Point(i + cellsize, j + cellsize, false);
          points.add(fir);
            }
        }

    }

    public void printMaze(String[][] arr) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = 0; j < arr[i].length; j++) {
                System.out.print(arr[i][j]);
            }
            System.out.println();
        }
    }

    public void prim(int height, int width, int cellsize) {
        boolean[] mark = new boolean[points.size()];
        Queue<Point> que = new LinkedList<Point>();
       int rand = (int) (1+ (Math.random()*points.size()));
      mark[rand] = true;
      que.add(points.get(rand));
      mark[rand]=false;
       while(points.size()>0){
           
       rand=(int) (1+ (Math.random()*points.size()));
       if(mark[rand]==false){
       que.add(points.get(rand));
       addlines(height, width, cellsize, que);
       mark[rand]=false;
       }

       }

    }

    public void addlines(int height, int width, int cellsize,Queue<Point> que) {
       Point wr = que.remove();
        int xf=wr.xc;
        int yf=wr.yc;
       
        if(xf+cellsize < width ){
            StdDraw.line(xf, yf, xf+cellsize,yf );
        }
        if(yf+cellsize <= height){
            StdDraw.line(xf, yf, xf,yf + cellsize);
        }
    }

    public static void main(String[] args) {

        int width = Integer.parseInt(args[0]); // the colum
        int height = Integer.parseInt(args[1]); // the row
        int cellsize = Integer.parseInt(args[2]);
        tut5 maze = new tut5();
        
        maze.makeBoard(height, width, cellsize);
       

    }
}