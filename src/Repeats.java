
import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Scanner;

/**
 * the class for the repeats
 */

public class Repeats {

    // Global variables
    public static String nm = "";
    public static ArrayList<String> xyx = new ArrayList<String>();

    // instance variables

    String dna = "";

    /**
     * it is a constructor that initialises the dna sequence
     * @param dna is the string to set the instance variable to
     */
    public Repeats(String dna) {
        this.dna = dna;
    }

    /**
     * the nested class 
     */
    public static class Node {

        // global variable
        public static String nm3 = "";
        List<Node> node = new LinkedList<Node>();

        // instance variable
        int depth;
        String start;
        String subString;
        boolean rept = true;

        /**
         * is an constructor that makes the Node object
         * @param ch  the char to initialise the char instance variable
         * @param rpt the boolean of the Node
         * @param sub the String of the Node
         * @param dp  the depth of the Node
         */
        public Node(String ch, boolean rpt, String sub, int dp) {

            this.rept = rpt;
            this.start = ch;
            this.subString = sub;
            this.depth = dp;
        }

        /**
         * a constructor
         */
        public Node() {

        }

        /**
         * it changes the value of the boolean of the node
         * @param toset is the value of the boolean i want to change to
         */
        public void setRept(boolean toset) {
            this.rept = toset;
        }

        /**
         * this method gets the value of the boolean in the node
         * @return the boolean value of node
         */
        public boolean getRept() {
            return this.rept;
        }

        /**
         * this method returns the current substring in the node
         * @return a string in the node
         */
        public String getSubstring() {
            return this.subString;
        }

        /**
         * this method gets the value of the depth of the node
         * @return int which is the depth of the node
         */
        public int getDepth() {
            return this.depth;
        }

        /**
         * this method makes a tree of strings where there are no repitiions
         * @param alphSize is the number of chars used in sequence
         * @param startCh  is the char that we start the sequences with
         * @param dpth     is max depth of the srtings
         */
        public void mode2(int alphSize, String startCh, int dpth) {
            try {
                int no = 1;
                //gets the file name
                File out = new File("../out/" + "gen" + no + "_bf.txt");
                while (!out.createNewFile()) {
                    no = no + 1;
                    out = new File("../out/" + "gen" + no + "_bf.txt");
                }
                FileWriter output = new FileWriter(out);
                Queue<Node> que = new LinkedList<Node>();
                int terminate = dpth;
                if (dpth == 0) {
                    Node root = new Node(startCh, true, startCh, 0);
                    Node pointer = root;
                    output.write(1 + " - " + root.getSubstring() + "\n");
                    addChild0(root, alphSize, output);
                    while (pointer.rept) {

                        for (int i = 0; i < alphSize; i++) {
                            que.add(pointer.node.get(i));
                        }
                        pointer = que.remove();
                        while (!pointer.rept && que.size() > 0) {
                            pointer = que.remove();
                        }
                        addChild0(pointer, alphSize, output);
                    }
                } else if (dpth > 1) {
                    Node root = new Node(startCh, true, startCh, 1);
                    Node pointer = root;
                    output.write(1 + " - " + root.getSubstring() + "\n");
                    addChild(root, alphSize, dpth, output);
                    while (pointer.depth < dpth && pointer.rept) {
                        for (int i = 0; i < alphSize; i++) {
                            que.add(pointer.node.get(i));
                        }
                        pointer = que.remove();
                        while (!pointer.rept && que.size() > 0) {
                            pointer = que.remove();
                        }
                        addChild(pointer, alphSize, terminate, output);

                    }
                }
                output.close();
            } catch (Exception e) {
            }
        }

        /**
         * it adds children to the parent
         * @param pointer  is the parent node
         * @param alphSize is the number of chars used in sequence
         * @param dept     is max depth of the srtings
         * @param output   is the text file i must write my output to
         */
        public void addChild(Node pointer, int alphSize, int dept,
                FileWriter output) {
            try {
                String alphChars = "ACGT";
                String str = "";
                for (int i = 0; i < alphSize; i++) {
                    String toadd = "" + alphChars.charAt(i);

                    String aa = pointer.getSubstring();
                    int dpt = pointer.getDepth() + 1;
                    if (!rptm3(aa, alphChars.charAt(i)) && dpt <= dept) {
                        String txt = pointer.getSubstring().concat(toadd);
                        pointer.node.add(new Node(toadd, true, txt, dpt));
                        str = str + txt.length() + " - " + txt + "\n";
                    } else if (rptm3(aa, alphChars.charAt(i)) && dpt <= dept) {
                        String txt = pointer.getSubstring().concat(toadd);
                        pointer.node.add(new Node(toadd, false, txt, dpt));
                    }
                }
                output.write(str);

            } catch (Exception e) {
            }
        }

        /**
         * it adds children to the parent
         * 
         * @param pointer  is the parent node
         * @param alphSize is the number of chars used in sequence
         * @param output   is the text file i must write my output to
         */

        public void addChild0(Node pointer, int alphSize, FileWriter output) {
            try {
                String alphChars = "ACGT";
                String str = "";
                for (int i = 0; i < alphSize; i++) {
                    String toadd = "" + alphChars.charAt(i);
                    String txt = pointer.getSubstring();
                    if (!rptm3(txt, alphChars.charAt(i))) {
                        String aa = pointer.getSubstring().concat(toadd);
                        pointer.node.add(new Node(toadd, true, aa, 0));
                        str = str + aa.length() + " - " + aa + "\n";
                    } else if (rptm3(txt, alphChars.charAt(i))) {
                        pointer.node.add(new Node(toadd, false, txt, 0));
                    }
                }
                output.write(str);
            } catch (Exception e) {
            }
        }

        /**
         * this method checks if the string has xyx repititions
         * 
         * @param txt the string i want to check for repititions
         * @param chr is the char of the to be added in the text
         * @return false if there is no repetitions and true otherwise
         */
        public boolean rptm3(String txt, char chr) {
            int txleng = txt.length() - 1;
            while (txleng > 0 && txt.charAt(txleng) != chr) {
                txleng -= 1;
            }
            String pat = txt.substring(txleng, txt.length()) + chr;
            if (validate(txt + chr, pat)) {
                return true;
            }
            return false;
        }

        /**
         * this method cheks for the repetitions of the pattern in the string
         * 
         * @param txt is the string i am checking if there is repititions
         * @param pt  is the repesting string i am looking for
         * @return true if there is repetition and false otherwise
         */
        public boolean validate(String txt, String pt) {
            int count = 0;
            int m = pt.length();
            int n = txt.length();
            for (int i = 0; i <= n - m; i++) {
                int j;
                for (j = 0; j < m; j++) {
                    if (txt.charAt(i + j) != pt.charAt(j)) {
                        break;
                    }
                }
                if (j == m) {
                    count = count + 1;
                }
                if (count > 1) {
                    return true;
                }
            }
            return false;
        }

        /**
         * gets the longest string with no repetition in the spcified time
         * 
         * @param alphsize is the number of alphabets going to be use
         * @param strch    is the char used at start point
         * @param time     is the amount of time given to get the string
         * @param timer    is the the reference to the stopwatch in main
         */

        public void mode3(int alphsize, String strch, double time,
         Stopwatch timer) {
            Node root = new Node(strch, true, strch, 0);
            Node pointer = root;
            addChildm3(root, alphsize);
            Queue<Node> que = new LinkedList<Node>();
            que.add(pointer.node.get(0));
            while (timer.elapsedTime() <= time * 0.9&& que.size() > 0) {
                if (pointer.rept) {
                    for (int i = 0; i < alphsize; i++) {
                        Node ad = pointer.node.get(i);
                        if (ad.rept) {
                            que.add(ad);
                        }
                    }
                    pointer = que.remove();
                    addChildm3(pointer, alphsize);
                }
            }
            try {
                //get the file name
                int no = 1;
                File out = new File("../out/" + "out" + no + "_opt.txt");
                while (!out.createNewFile()) {
                    no = no + 1;
                    out = new File("../out/" + "out" + no + "_opt.txt");
                }
                FileWriter output = new FileWriter(out);
                output.write(nm3.length() + " - " + nm3);
                output.close();
            } catch (Exception e) {

            }
        }

        /**
         * adds children to the root node
         * 
         * @param pointer  is the root node i am adding childern
         * @param alphsize is the number of aphabets i must use
         */
        public void addChildm3(Node pointer, int alphsize) {
            String alphChars = "ACGT";
            for (int i = 0; i < alphsize; i++) {
                String toadd = "" + alphChars.charAt(i);
                String txt = pointer.getSubstring();
                if (!rptm3(txt, alphChars.charAt(i))) {
                    String aa = pointer.getSubstring().concat(toadd);
                    if (nm3.length() < aa.length()) {
                        nm3 = aa;
                    }
                    pointer.node.add(new Node(toadd, true, aa, 0));
                } else if (rptm3(txt, alphChars.charAt(i))) {
                    String aa = pointer.getSubstring().concat(toadd);
                    pointer.node.add(new Node(toadd, false, aa, 0));
                }
            }
        }
    }

    /**
     * checks if the dna string has repitions of the sub
     * 
     * @param dna  the string which i must check for repititions
     * @param sub  is the sustring i must check for its repitition in the dna
     * @param bign is int which is the begining of the sub
     */
    public void repeating(String dna, String sub, int bign) {
        int gap = sub.length();
        int count = 0;
        String check = "";
        ArrayList<Integer> indx = new ArrayList<Integer>();
        for (int i = bign + 1; i < dna.length(); i++) {
            char sb = dna.charAt(i);
            if (sub.charAt(0) == sb) {
                try {
                    check = dna.substring(i - gap + 1, 1 + i);
                } catch (Exception e) {
                    continue;
                }
                if (sub.equals(check)) {
                    count = count + 1;
                    if (sub.equals(check) && count >= 2 && !(bign 
                    == (i - gap + 1))) {
                        indx.add(bign);
                        indx.add(i - gap + 1);
                    }
                }
            }
        }
        if (count >= 2) {
            try {
                FileWriter output = new FileWriter("../out/" + nm, true);
                String str = "";
                for (int i = 0; i < indx.size(); i += 2) {
                    try {
                        str = str + sub + " " + indx.get(i) + " " 
                               + indx.get(i + 1) + "\n";
                        xyx.add(sub);

                    } catch (Exception e) {
                        continue;
                    }
                }
                output.write(str);
                output.close();
            } catch (Exception e) {
            }
        }
    }

    /**
     * makes the name of the text file
     * @param b is the name in my inputstream
     */
    public void names1(String b) {
        String filename = "";
        for (int i = 0; i < b.length(); i++) {
            String chrs = "" + b.charAt(i);
            if (chrs.equals(".")) {
                break;
            }
            filename = filename + chrs;
        }
        String toadd = "_chk.txt";
        nm = filename.concat(toadd);
    }

    /**
     * it cheks if the dna string has repititions of xyx
     * 
     * @param dna is the string of the thing i must check for repititions
     */
    public void mode1(String dna) {
        int cel = (int) Math.ceil((dna.length() / 2)) + 1;
        for (int i = 0; i < dna.length() - 1; i++) {
            String xa = "" + dna.charAt(i);
            if (xa.equals("A") || xa.equals("C") || xa.equals("G") 
                    || xa.equals("T")) {
                for (int j = 1 + i; j < dna.length(); j++) {
                    String xb = "" + dna.charAt(j);
                    if (xa.equals(xb)) {
                        String sub = dna.substring(i, 1 + j);
                        if (sub.length() <= cel && sub.length() > 1) {
                            repeating(dna, sub, i);
                        }
                    }
                }
            } else {
                System.err.println("ERROR: invalid alphabet symbol");
                break;
            }
        }
        if (xyx.size() == 0) {
            try {
                FileWriter output = new FileWriter("../out/" + nm, true);
                output.write("None");
                output.close();
            } catch (Exception e) {
            }
        }
    }

    /**
     * is the main method
     * @param args is the string array that contains the ar
     */
    public static void main(String[] args) {
        int mode = 0;
        //REMARK
        Stopwatch timer = new Stopwatch();
        if (args.length == 0) {
            System.err.println("ERROR: invalid number of argument");
            System.exit(0);
        }

        try {
            mode = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.err.println("ERROR: invalid argument type");
            System.exit(0);
        }

        // mode 1
        if (mode == 1) {
            int leng = args.length;
            if (leng != 2) {
                System.err.println("ERROR: invalid number of argument");
                System.exit(0);
            }
            try {
                String dna = args[1];
                Repeats xyxs = new Repeats(dna);
                try {
                    File file = new File(dna);
                    String sfile = file.getName();
                    xyxs.names1(sfile);
                    Scanner sc = new Scanner(file);
                    while (sc.hasNextLine()) {
                        dna = sc.nextLine();
                    }
                    sc.close();
                    xyxs.mode1(dna);
                } catch (Exception e) {
                    System.err.println("ERROR: invalid or missing file");
                    System.exit(0);
                }
            } catch (Exception e) {
                System.err.println("ERROR: invalid number of argument");
                System.exit(0);
            }

            // MODE 2
        } else if (mode == 2) {
            int alphSize = 0;
            int depth = 0;
            int leng = args.length;
            String startCh = "Q";
            if (leng != 4) {
                System.err.println("ERROR: invalid number of argument");
                System.exit(0);
            }
            try {
                alphSize = Integer.parseInt(args[1]);
                startCh = args[2];
                depth = Integer.parseInt(args[3]);
            } catch (Exception e) {
                System.err.println("ERROR: invalid argument type");
                System.exit(0);
            }
            if ((startCh.equals("A")) || (startCh.equals("C")) 
                 || (startCh.equals("G")) || (startCh.equals("T"))) {
                Node xyxs = new Node();
                xyxs.mode2(alphSize, startCh, depth);
            } else {
                System.err.println("ERROR: invalid alphabet symbol");
                System.exit(0);
            }

            // MODE 3 SECOND HAND-IN
        } else if (mode == 3) {
            int alphSize = 0;
            double time = 0;
            int leng = args.length;
            String startCh = "Q";
            if (leng != 4) {
                System.err.println("ERROR: invalid number of argument");
                System.exit(0);
            }
            try {
                alphSize = Integer.parseInt(args[1]);
                startCh = args[2];
                time = Double.parseDouble(args[3]);
            } catch (Exception e) {
                System.err.println("ERROR: invalid argument type");
                System.exit(0);
            }
            if ((startCh.equals("A")) || (startCh.equals("C")) 
                 || (startCh.equals("G")) || (startCh.equals("T"))) {
                Node xyx = new Node();
                xyx.mode3(alphSize, startCh, time, timer);

            } else {
                System.err.println("ERROR: invalid alphabet symbol");
                System.exit(0);
            }
        } else {
            System.err.println("ERROR: invalid mode");
            System.exit(0);
        }
    }
}
