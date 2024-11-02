package myTemporalapp;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
 
/**
 * Contains all business functions 
 * 
 * @author Koni 
 */
public class API2Methods implements API2MethodsInfrc{
    private Connection conn = null;
    private final static String myTableName = "api2tablek";
    private final static String myUserName = "konanani";
    private final static String myPassword = "123456";
    private final static String myDatabase = "api2databasek";
    private static String[] myColumnName = {"Transaction ID", "Sender ID", "Transaction Amount", "Transaction Data", "Transaction Time", "Transaction Status"};
 
    /**
     * Connects to the business database
     * 
     * @param dbname name of database
     * @param user username of owner
     * @param password password of owner
     */
    @Override
    public Connection connect_to_db(String dbname, String user, String password) {
        try {
            Class.forName("org.postgresql.Driver");
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/" + dbname, user, password);
            System.out.println(conn);
            if (conn != null) {
                System.out.println("Connection to API2 Database established");
            } else {
                System.out.println("Connection to API2 Database failed");
            }
        } catch (Exception e) {
            System.out.println(e);
            //System.out.println("conn is null");
            //System.err.println("conn is null");
        }
        return conn;
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
            System.out.println("Error: " + e.getMessage());
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
            System.out.println("Error: " + e.getMessage());
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
            System.out.println("Error: " + e.getMessage());
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
 
        Connection conn = connect_to_db("api2database", "postgres", "123456");
        int senderId = 2; //TODO you must get the sender id from api1
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
 
        Connection conn = connect_to_db("api2databasek", "konanani", "123456");
        int senderId = 2; //TODO you must get the sender id from api1
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
 
        Connection conn = connect_to_db("api2database", "postgres", "123456");
        int senderId = 2; //TODO you must get the sender id from api1
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
        Connection conn = connect_to_db("api2database", "postgres", "123456");
        int senderId = 2; //TODO you must get the sender id from api1
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
        Connection conn = connect_to_db("api2database", "konanani", "123456");
        int senderId = 2; //TODO you must get the sender id from api1
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
                System.out.println("fail here 1");
                System.out.println("Failed to record transaction");
                return "FAILED";
            }
        } catch (Exception e) {
            System.out.println("fail here 2");
            System.out.println("Error: " + e.getMessage());
            return "FAILED";
        } finally {
            try {
                if (preparedStatement != null) {
                    preparedStatement.close();
                }
            } catch (Exception e) {
                // Handle exception if closing the statement fails
                System.out.println("fail here 3");
                System.out.println("Error performing transaction: " + e.getMessage());
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
                System.out.println("status updated successfully.");
                return true;
            } else {
                System.out.println("status not updated ");
                return false;
            }
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
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
}