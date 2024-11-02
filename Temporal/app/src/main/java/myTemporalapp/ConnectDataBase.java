package myTemporalapp;

import java.sql.Connection;
import java.time.LocalDateTime;
import java.io.BufferedReader;
import java.io.FileReader;


public class ConnectDataBase {
    public static void main(String[] args) {
        String db2name = "api2databasek";
        String db3name = "api3database";
        String owner = "konanani";
        String password = "123456";

        int[] data = {50, 100, 500, 1000, 2000};
        int[] dataPrice = {15, 25, 60, 100, 170};

        //Scanner scanner = new Scanner(System.in);
        TemporalMethods tempMethods = new TemporalMethods();
        String productType = "";
        int amount = 0;
        int amountProduct = 0;
        String phoneNumber = "";
        String PAN = "";
        //TODO reading from a textfile
        //File inputFile = new File("./input");
        try {
            FileReader inputFile = new FileReader("input.txt");
            BufferedReader bufferedReader = new BufferedReader(inputFile);

            String line;

            // Read and print each line in the file until the end of the file is reached
            while ((line = bufferedReader.readLine()) != null) {
                String[] arr = line.split(":");
                if (arr[0].equals("productType")) {
                    productType = arr[1];//"airtime";
                } else if (arr[0].equals("amount")){
                    amount = Integer.parseInt(arr[1]);//25;
                }else if(arr[0].equals("amountProduct")){
                    amountProduct =  Integer.parseInt(arr[1]);//50;
                }else if(arr[0].equals("phoneNumber")){
                    phoneNumber =  arr[1];//"1231231234";
                }else if(arr[0].equals("PAN")){
                    PAN = arr[1];//"WXYZA5678B";
                }
                bufferedReader.close();
            }
        } catch (Exception e) {
            productType ="airtime";
            amount = 25;
            amountProduct = 50;
            phoneNumber = "1231231234";
            PAN = "WXYZA5678B";
        }
        
        // while () {
            
        // }
        // while(true){
        //     System.out.println("Do you want to purchase data or airtime? (data/airtime)");
        //     productType = scanner.nextLine();

        //     if(productType.equals("airtime")){
        //         System.out.println("How much airtime do you want?");
        //         amountProduct = scanner.nextInt();
        //         break;
        //     } else if(productType.equals("data")){
        //         System.out.println("How many MBs of data do you want? (50, 100, 500, 1000, 2000)");
        //         amountProduct = scanner.nextInt();
        //         break;
        //     } else {
        //         System.out.println("Incorrect product type");
        //     }
        // }

        // if(productType.equals("data")){
        //     for(int i = 0; i < data.length; i++){
        //         if(amountProduct == data[i]) {
        //             amount = dataPrice[i];
        //             break;
        //         }
        //     }
        // } else {
        //     amountProduct = amount;
        // }

        // System.out.println("Enter your phone number:");
        // phoneNumber = scanner.next();
        // //if (!phoneNumber.equals("0795847196") && !phoneNumber.equals("0854035986") && !phoneNumber.equals("0385749208") && !phoneNumber.equals("0532857537") && !phoneNumber.equals("0792868245")) {
 
        // //}

        // System.out.println("Enter PAN");
        // String PAN = scanner.next();
        //  //if (!PAN.equals("0001") && !PAN.equals("0010") && !PAN.equals("0011") && !PAN.equals("0100") && !PAN.equals("0101")) {
        // // System.out.println("PAN invalid");
        // // return;
        // //}

        // scanner.close();
        

        // Connect to the API1 database
        //customerBank db1 = new customerBank();
        API1Methods db1 = new API1Methods();
        db1.connect_to_db("api1databasek", "konanani", "123456");

        // Perform a payment transaction
        //String paymentStatus = db1.transaction(PAN, amount, "PAYMENT");
        
        //TODO using temporal
        String date = get_date();
        String time = get_time();
        String paymentStatus = tempMethods.transactionTemp(PAN,amount,"PAYMENT",time,db1);
        System.out.println("THE payment status is "+paymentStatus);

        
        if (paymentStatus.equals("FAILED") || paymentStatus.equals("ERROR")) {
            System.out.println("Error transmitting funds to business\n");
            return;
        }

        if (paymentStatus == "SUCCESS") {
            System.out.println("payment was sucessful");
            // Connect to the API2 database
             API2Methods db2 = new API2Methods();
             Connection conn2 = db2.connect_to_db(db2name, owner, password);
             int userID = 5;
            // int userID = db1.get_user_id(PAN);

            // if (userID == -1) {
            //     return;
            // }

            date = get_date();
            time = get_time();

            // Add a transaction record in the API2 database
            //String transSucOrFail = db2.addTrans(conn2, userID, amount, date, time);
            //TODO temporal
            String transSucOrFail = tempMethods.addTransTemp(userID,amount,date,time,PAN,db2);
            // System.out.println("+++++++++++++++++++++++++++++++++++++");
            // System.out.println(transSucOrFail);
            // System.out.println("++++++++++++++++++++++++++++++++++++++");
            if (transSucOrFail == "FAILED") {
                // If transaction failed, perform a reversal
                //String reversalStatus = db1.transaction(PAN, amount, "REVERSAL");
                //TODO add temporal
                String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
                
                if (reversalStatus == "SUCCESS") {
                    System.out.println("Failed 2");
                    System.out.println("Failed transaction, funds returned.");
                }
            }

            if (transSucOrFail.equals("SUCCESS")) {
                // Connect to the API3 database
                API3Methods db3 = new API3Methods();
                db3.connect_to_db(db3name, owner, password);
 
                if (productType.equals("airtime")) {
                    // Perform airtime purchase and update transaction status
                    //String airtimePurchase = db3.purchaseAirtime(phoneNumber, date, time, amountProduct);
                    //TODO temporal
                    String airtimePurchase = tempMethods.purAirtmTemp(phoneNumber,date,time,amountProduct,PAN,db3);
                   
                    
                    if (airtimePurchase.equals("SUCCESS")) {
                        db2.updateTransStatus(date, time, "Completed");
                        //TODO temporal
                        tempMethods.updateTransTemp(conn2,date,time,"Completed",PAN,db2);
 
                    } else if (airtimePurchase.equals("FAILED")) {
                        db2.updateTransStatus(date, time, "Failed");
                        //TODO temporal
                        tempMethods.updateTransTemp(conn2,date,time,"Failed",PAN,db2);

                         //TODO temporal
                        //String reversalStatus = db1.transaction(PAN, amount, "REVERSAL");
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);

                        if (reversalStatus.equals("SUCCESS")) {
                            System.out.println("Failed 1");
                            System.out.println("Failed transaction, funds returned.\n");
                        }
 
                    } else if (airtimePurchase.equals("ERROR")) {
                        //String reversalStatus = db1.transaction(PAN, amount, "REVERSAL");
                        //TODO temporal
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);


                        //db2.updateTransStatus(conn2, date, time, "Failed");
                        //TODO temporal stuff
                        tempMethods.updateTransTemp(conn2,date,time,"Failed",PAN,db2);

                        if (reversalStatus.equals("SUCCESS")) {
                            System.out.println("Failed transaction, funds returned.\n");
                        }
                    }
                }
 
                if (productType.equals("data")) {
                    // Perform data purchase and update transaction status
                    String dataPurchase = db3.purchaseData(phoneNumber, date, time, amountProduct);
 
                    if (dataPurchase.equals("SUCCESS")) {
                        //db2.updateTransStatus(conn2, date, time, "Completed");
                        //TODO temporal
                        tempMethods.updateTransTemp(conn2,date,time,"Completed",PAN,db2);
                       

                    } else if (dataPurchase.equals("FAILED")) {
                        //db2.updateTransStatus(conn2, date, time, "Failed");
                        //TODO temporal 
                        tempMethods.updateTransTemp(conn2,date,time,"Failed",PAN,db2);
                        
                        //String reversalStatus = db1.transaction(PAN, amount, "REVERSAL");
                        //TODO temporal
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);

                        if (reversalStatus.equals("SUCCESS")) {
                            System.out.println("Failed transaction, funds returned.\n");
                        }
 
                    } else if (dataPurchase.equals("ERROR")) {
                        //db2.updateTransStatus(conn2, date, time, "Failed");
                        //TODO temporal stuff
                        tempMethods.updateTransTemp(conn2,date,time,"Failed",PAN,db2);


                        //String reversalStatus = db1.transaction(PAN, amount, "REVERSAL");
                        //TODO temporal
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);

                        if (reversalStatus.equals("SUCCESS")) {
                            System.out.println("Failed transaction, funds returned.\n");
                        }
                    }
                }
            }
        }
        System.exit(0);
    }

    public static String get_time() {

        LocalDateTime currentDateTime = LocalDateTime.now();
        int hour = currentDateTime.getHour();
        int minute = currentDateTime.getMinute();
        int second = currentDateTime.getSecond();
        return String.format("%02d:%02d:%02d", hour, minute, second);
    }

    public static String get_date() {
        LocalDateTime currentDateTime = LocalDateTime.now();
        int year = currentDateTime.getYear();
        int month = currentDateTime.getMonthValue();
        int day = currentDateTime.getDayOfMonth();
        return String.format("%04d-%02d-%02d", year, month, day);
    }

    
}

