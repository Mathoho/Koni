import java.util.Scanner;

/**
 * @author Mathoho Konanani
 */
public class Greeter {

    /**
     * prints a a greeting in the terminal
     */
    public static void greeter(){
        System.out.print("hello ");
    }

    public static void goodbye(){
        System.out.println("bye");
    }
    /**
     * it gets the name from the user input
     * @return the name of the person
     */
    public static String getName(){
        Scanner scan = new Scanner(System.in);
        String nme = scan.nextLine();
        scan.close();
        return nme;
    } 
    /**
     * the main method
     * @param args the array of commandline arguments
     */
    public static void main(String[] args){
        String name = getName();
        greeter();
        System.out.println(name);
    }
}