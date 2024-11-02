package VertNTemp;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import io.vertx.core.json.JsonObject;
 
/**
 * Contains all business functions 
 * 
 * @author Koni 
 */
public class API2Methods implements API2Interface{
    private Connection conn = null;
    private final static String myTableName = "api2tablek";
   
 
    /**
     * Connects to the business database
     * 
     * @param dbname name of database
     * @param user username of owner
     * @param password password of owner
     */
    @Override
    public void connect_to_db(String dbname, String user, String password) {
        try {
            Class.forName("org.postgresql.Driver");
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/" + dbname, user, password);
            if (conn != null) {
                System.out.println("Connection to API2 Database established");
            } else {
                System.out.println("Connection to API2 Database failed");
            }
        } catch (Exception e) {
           // System.out.println(e);
        }
    }
 
    /**
     * Gets a specified integer element from the database table 
     * 
     * @param conn the connection object used to connect to the datbase
     * @param itemColumn the element to get
     * @param SenderID the customer ID number according to business records
     * @return the element
     */
    private int getIntItem(String itemColumn, int SenderID) {
        PreparedStatement preparedStatement = null;
        int item = 0;
        try {
            String query = String.format("SELECT %s FROM %s WHERE Sender ID = ?", itemColumn, myTableName);
 
            preparedStatement = conn.prepareStatement(query);
            preparedStatement.setInt(1, SenderID);
 
            ResultSet rs = preparedStatement.executeQuery();
            rs.next();
            item = rs.getInt(itemColumn);
        } catch (Exception e) {
            //System.out.println("Error: " + e.getMessage());
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                // Handle exception if closing the statement fails
 
            }
        }
        return item;
    }
 
    /**
     * Gets a specified string element from the database table 
     * 
     * @param conn the connection object used to connect to the datbase
     * @param itemColumn the element to get
     * @param SenderID the customer ID number according to business records
     * @return the element
     */
    private String getStrItem(String itemColumn, int SenderID) {
        PreparedStatement preparedStatement = null;
        String item = "";
        try {
            String query = String.format("SELECT %s FROM %s WHERE Sender ID = ?", itemColumn, myTableName);
 
            preparedStatement = conn.prepareStatement(query);
            //int num = Integer.parseInt(SenderID);
            preparedStatement.setInt(1, SenderID);
 
            ResultSet rs = preparedStatement.executeQuery();
            rs.next();
            item = rs.getString(itemColumn);
        } catch (Exception e) {
            //System.out.println("Error: " + e.getMessage());
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                // Handle exception if closing the statement fails
 
            }
        }
        return item;
    }
 
    /**
     * Gets a specified double element from the database table
     *  
     * @param conn the connection object used to connect to the datbase
     * @param itemColumn the element to get
     * @param SenderID the customer ID number according to business records
     * @return the element
     */
    private double getDoubleItem(String itemColumn, String[] conditions, int SenderID) {
        PreparedStatement preparedStatement = null;
        Double item = 0.0;
        try {
            String query = String.format("SELECT %s FROM %s WHERE Sender ID = ?", itemColumn, myTableName);
 
            preparedStatement = conn.prepareStatement(query);
            preparedStatement.setInt(1, SenderID);
 
            ResultSet rs = preparedStatement.executeQuery();
            rs.next();
            item = rs.getDouble(itemColumn);
        } catch (Exception e) {
            //System.out.println("Error: " + e.getMessage());
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                // Handle exception if closing the statement fails
 
            }
        }
        return item;
    }
 
    /**
     * Gets the ID number of a transaction
     * 
     * @return the ID number
     */
    public int getTransID() {
        int senderId = 2; //TODO you must get the sender id from api1
        int id = getIntItem("Transaction ID", senderId);
        return id;
 
    }
 
    /**
     * Gets customer ID
     * 
     * @return customer ID
     */
    @Override
    public int getSenderIDapi2() {
        int senderId = 2; 
        int id = getIntItem("Sender ID", senderId);
        return id;
 
    }
 
 
    /**
     * Gets the amount of the transaction
     * 
     * @return amount
     */
    @Override
    public double getTransAmount() {
        int senderId = 2; 
        double amount = getDoubleItem("Transaction Amount", null, senderId);
        return amount;
 
    }
 
    /**
     * Gets the data of the transaction
     * 
     * @param conditions
     * @return the data
     */
    @Override
    public String getTransData(String[] conditions) {
        int senderId = 2; 
        String data = getStrItem("Transaction Data", senderId);
        return data;
 
    }
 
 
    /**
     * Gtes the time if the transaction
     * 
     * @return the time
     */
    @Override
    public String getTranstime() {
        int senderId = 2;
        String time = getStrItem("Transaction Data", senderId);
        return time;
 
    }
 
    /**
     * Gets the status of the transaction
     * 
     * @param conditions
     * @return the status
     */
    @Override
    public String getTransStatus(String[] conditions) {
        int senderId = 2; 
        String status = getStrItem("Transaction Data", senderId);
        return status;
 
    }
//===========================================================INSERT FUNCTION===========================================================================
 
    /**
     * Processes transaction from business side
     * 
     * @param conn connection object used to connect to the database
     * @param senderId customer ID umber according to business records
     * @param transAmount amount of transaction
     * @param transDate date of transaction
     * @param transTime time of transaction 
     * @return status of transaction
     */
    @Override
    public String addTrans(int senderId, double transAmount, String transDate, String transTime) {
        PreparedStatement preparedStatement = null;
        String transStatus = "Pending";
 
        try {
            String query = "INSERT INTO " + myTableName + " ( \"Sender ID\", \"Transaction Amount\", \"Transaction Date\", \"Transaction Time\", \"Transaction Status\") " +
                    "VALUES (?, ?, ?, ?, ?)";
            
            preparedStatement = conn.prepareStatement(query);
            preparedStatement.setInt(1, senderId);
            preparedStatement.setDouble(2, transAmount);
            preparedStatement.setString(3, transDate);
            preparedStatement.setString(4, transTime);
            preparedStatement.setString(5, transStatus);
 
            int rowsInserted = preparedStatement.executeUpdate();
 
            if (rowsInserted > 0) {
                System.out.println("Transaction recorded successfully");
                return "SUCCESS";
            } else {
                System.out.println("Failed to record transaction");
                return "FAILED";
            }
        } catch (Exception e) {
            return "FAILED";
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                return "ERROR";
 
            }
        }
    }
 
    /**
     * Updates the status of a transaction 
     * 
     * @param conn connection object used to connect to the database
     * @param date date of transaction 
     * @param time time of transaction 
     * @param status the status to which the transaction should be updated
     * @return true if successful, false if not
     */
    @Override
    public boolean updateTransStatus(String date, String time, String status) {
        PreparedStatement preparedStatement = null;
 
        try {
            String query = "UPDATE " + myTableName + " SET \"Transaction Status\" = ? WHERE  \"Transaction Date\" = ? AND \"Transaction Time\"= ?";
            preparedStatement = conn.prepareStatement(query);
            preparedStatement.setString(1, status);
            preparedStatement.setString(2, date);
            preparedStatement.setString(3, time);
 
            int rowsInserted = preparedStatement.executeUpdate();
 
            if (rowsInserted > 0) {
                System.out.println("Transaction Status updated successfully.");
                return true;
            } else {
                System.out.println("Transaction Status not updated ");
                return false;
            }
        } catch (Exception e) {
            //System.out.println("Error: " + e.getMessage());
            return true;
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                // Handle exception if closing the statement fails
                return false;
            }
        }
    }

   //get the user's transaction history given the senderid
   public JsonObject getUserTrasactionHist(int senderId){
        PreparedStatement preparedStatement = null;
        JsonObject transactionJson = new JsonObject();
        try {
            String query = "SELECT * FROM " + myTableName + " WHERE  \"Sender ID\" = ?";
            preparedStatement = conn.prepareStatement(query);
            preparedStatement.setInt(1, senderId);
            //int rowssel = preparedStatement.executeUpdate();
            ResultSet resultSet = preparedStatement.executeQuery();
            
            if (resultSet != null) {
                //make an array list of jason objects
                // Process the result set
                while (resultSet.next()) {
                    // Access and process the columns from the result set
                    int transactionId = resultSet.getInt("Transaction ID");
                    int senderID = resultSet.getInt("Sender ID");
                    double transactionAmount = resultSet.getDouble("Transaction Amount");
                    String transactionDate = resultSet.getString("Transaction Date");
                    String transactionTime = resultSet.getString("Transaction Time");
                    String transactionStatus = resultSet.getString("Transaction Status");
                    
                    transactionJson
                        .put("TransactionId", transactionId)
                        .put("SenderId", senderID)
                        .put("TransactionAmount", transactionAmount)
                        .put("TransactionDate", transactionDate)
                        .put("TransactionTime", transactionTime)
                        .put("TransactionStatus", transactionStatus);
                }
                return transactionJson;
            } else {
                transactionJson
                    .put("Error", "Failed to get the user\'s transaction History");
                return transactionJson;
            }
        } catch (Exception e) {
            transactionJson
                .put("Error", e.toString());
            return transactionJson;
        }
    } 

//    public static void main(String[] args) {
//     System.out.println("in the main method now");
//     API2Methods db2 = new API2Methods();
//         db2.connect_to_db("api2databasek", "konanani", "123456");
//         JsonObject val = db2.getUserTrasactionHist(1);
//         if (val != null) {
//             System.out.println("they have everything now");
//         }else{
//             System.out.println("they do not have ahything");
//         }
//    }
}