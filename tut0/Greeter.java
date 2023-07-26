import java.util.Scanner;

public class Greeter {

    public static void getName(){
        Scanner scan = new Scanner(System.in);
        String nme = scan.nextLine();
        System.out.println(nme);
        scan.close();
    }
    public static void main(String[] args) {
        getName();
    }
}