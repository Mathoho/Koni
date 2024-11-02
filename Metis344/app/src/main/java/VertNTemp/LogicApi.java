package VertNTemp;
import java.sql.Connection;
import java.time.LocalDateTime;
public class LogicApi {
    public String Logic(String items[]){
       //items[user, product,amount, pan, amountProduct, cellNo]
       //amountProduct means how much money the product cost
       //amount is how many MB data you are buying 
        String db3name = "api3database";
        String owner = "konanani";
        String password = "123456";

        TemporalMethods tempMethods = new TemporalMethods();
        //check the input given on the array
        String err = checkItems(items);
        if(err != null){
            return err;
        }
        //now non of them are null
        String user = items[0];
        String productType = items[1];
        int amount = Integer.parseInt(items[2]); 
        String phoneNumber = items[5];
        String PAN = items[3];
        

        //Connect to the API1, API2 and AP3 database
        API1Methods db1 = new API1Methods();
        db1.connect_to_db("api1databasek", "konanani", "123456");

        API2Methods db2 = new API2Methods();
        db2.connect_to_db("api2databasek", "konanani", "123456");

        String date = get_date();
        String time = get_time();
        //String paymentStatus = tempMethods.transactionTemp(PAN,amount,"PAYMENT",time,db1);
        //String paymentStatus = "Insufficient funds";
        //String paymentStatus = "FAILED";
        String paymentStatus = "SUCCESS";
        if (paymentStatus.equals("Insufficient funds")) {
            System.out.println(PAN+" has Insufficient funds");
        }
        if (paymentStatus.equals("FAILED") || paymentStatus.equals("ERROR")) {
            System.out.println("Error transmitting funds to business\n");
            return "Error transmitting funds to business";
        }

        if (paymentStatus == "SUCCESS") {
            System.out.println("payment was sucessful");
            int userID = 0;
            // int userID = db1.get_user_id(PAN);
            // if (userID == -1) {
            //     String errorStr = String.format("User %s with id = %d does not exixst",user, userID);
            //     return errorStr;
            // }

            date = get_date();
            time = get_time();

            // Add a transaction record in the API2 database
            //String transSucOrFail = tempMethods.addTransTemp(userID,amount,date,time,PAN,db2);
            //String transSucOrFail = "FAILED";
            String transSucOrFail = "SUCCESS";
            if (transSucOrFail == "FAILED") {
                //If transaction failed, perform a reversal
                String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
                 
                if (reversalStatus == "SUCCESS") {
                    System.out.println("Failed transaction, funds returned.");
                    String errorStr = String.format("Failed transaction, funds returned to user %s\' bank account.",user);
                    return errorStr;
                }
            }

            if (transSucOrFail.equals("SUCCESS")) {
                // Connect to the API3 database
                API3Methods db3 = new API3Methods();
                String pseudoName = "FalseName";
                db3.connect_to_db(db3name, owner, password);
 
                if (productType.equals("airtime")) {
                    // Perform airtime purchase and update transaction status
                   // String airtimePurchase = tempMethods.purAirtmTemp(phoneNumber,date,time,amount,PAN,db3);
                   String airtimePurchase = "SUCCESS";
                   //String airtimePurchase = "SUCCESS";
                    if (airtimePurchase.equals("FAILED")) {
                        //tempMethods.updateTransTemp(date,time,"Completed",PAN,db2);
 
                    } else if (airtimePurchase.equals("FAILED")) {
                        tempMethods.updateTransTemp(date,time,"Failed",PAN,db2);
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
                        if (reversalStatus.equals("SUCCESS")) {
                            String errorStr = String.format("Failed transaction, funds returned to user %s\' bank account.",user);
                            return errorStr;
                        }
 
                    } else if (airtimePurchase.equals("ERROR")) {
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);

                        tempMethods.updateTransTemp(date,time,"Failed",PAN,db2);

                        if (reversalStatus.equals("SUCCESS")) {
                            String errorStr = String.format("Failed transaction, funds returned to user %s\' bank account.",user);
                            return errorStr;
                        }
                    }
                   
                }
 
                if (productType.equals("data")) {
                    int Value = Integer.parseInt(items[4]); //the MB of data
                    String dataPurchase = tempMethods.purDataTemp(phoneNumber, date, time, amount, PAN,db3);
                    if (dataPurchase.equals("SUCCESS")) {
                        tempMethods.updateTransTemp(date,time,"Completed",PAN,db2);
                       
                    } else if (dataPurchase.equals("FAILED")) {
                        tempMethods.updateTransTemp(date,time,"Failed",PAN,db2);
                        
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
            
                        if (reversalStatus.equals("SUCCESS")) {
                            String errorStr = String.format("Failed transaction, funds returned to user %s\' bank account.",user);
                            return errorStr;
                        }
 
                    } else if (dataPurchase.equals("ERROR")) {
                        tempMethods.updateTransTemp(date,time,"Failed",PAN,db2);
                        String reversalStatus = tempMethods.transactionTemp(PAN, amount, "REVERSAL",time,db1);
            
                        if (reversalStatus.equals("SUCCESS")) {
                            String errorStr = String.format("Failed transaction, funds returned to user %s\' bank account.",user);
                            return errorStr;
                        }
                    }
                   
                }
            }
        }
        String done = String.format("%s\' %s purchase transaction was sucessful ",user,productType);
        return done;
        
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

    public static boolean isValidNo(String num) {
        try {
            int nm = Integer.parseInt(num);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public static String checkItems(String[] item) {
        //items[user, product,amount, pan, value, cellNo]
        String product = item[1];
        String pan = item[3];
        String cellNo = item[5];
        String value = item[4];
        String amount = item[2];
       if(pan.equals("unknown")) {
            return "Please provide us with your (pan) Personal Account Number";
        }else if(product.equals("data") && value.equals("unknown")){
            return "Please provide us with the value of MB of data you want";
        }else if(cellNo.equals("unknown")){
            return  "Please provide us with the cellphone number to recharge their balance";
        }else if(product.equals("data") && !isValidNo(value)){
            return "Give a valid number for the value: Can not convert "+ value +" to a number";
        }else if(!isValidNo(amount)){
            return "Give a valid number for the amount: Can not convert "+ amount +" to a number";
        }
        if (product.equals("airtime") || product.equals("data")) {
            return null;
        } else{
            return "Invalid product type chose between buying airtime or data";
        }
    }

}