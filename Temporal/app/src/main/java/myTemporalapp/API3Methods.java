package myTemporalapp;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
 
/**
 * Contains all product functions
 * 
 * @author Stan Shikwambe
 * @author Jano Beukes
 */
public class API3Methods implements API3MethodsInfrc {
    private Connection conn;
    private final String table_name = "api3table";
 
    /**
     * Connects to the product database
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
 
            if (conn != null) {
                System.out.println("Connection to API3 Database established");
                System.out.println(conn);
 
            } else {
                System.out.println("Connection to API3 Database failed");
            }
        } catch (Exception e) {
            System.out.println(e);
        }
        return conn;
    }
 
    /**
     * Processes the purchasing of data by a customer
     * 
     * @param phoneNumber phone number onto whic the data should be loaded
     * @param date date of the purchase
     * @param time time of the purchase
     * @param data amount of data purchased
     * @return status of the process
     */
    @Override
    public String purchaseData(String phoneNumber, String date, String time, int data) {
 
        try {
            String sql = "UPDATE " + table_name + " SET \"Date Updated\" = ?, \"Data\" = ?, \"Time Updated\" = ? WHERE \"Phone Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(sql);
            preparedStatement.setString(1, date);
            preparedStatement.setInt(2, data);
            preparedStatement.setString(3, time);
            preparedStatement.setString(4, phoneNumber);
 
            int rowsUpdated = preparedStatement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("Transaction successful. Data Updated = " + data);
                return "SUCCESS";
            } else {
                System.out.println("Transaction failed.");
                return "FAILED";
            }
        } catch (Exception e) {
            System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
 
    }
 
     /**
     * Processes the purchasing of airtime by a customer
     * 
     * @param phoneNumber phone number onto whic the airtime should be loaded
     * @param date date of the purchase
     * @param time time of the purchase
     * @param airtime amount of airtime purchased
     * @return status of the process
     */
    @Override
    public String purchaseAirtime(String phoneNumber, String date, String time, int airtime) {
        try {
            String sql = "UPDATE " + table_name + " SET \"Date Updated\" = ?, \"Airtime\" = ?, \"Time Updated\" = ? WHERE \"Phone Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(sql);
            preparedStatement.setString(1, date);
            preparedStatement.setInt(2, airtime);
            preparedStatement.setString(3, time);
            preparedStatement.setString(4, phoneNumber);
 
            int rowsUpdated = preparedStatement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("Transaction successful. Airtime Updated = " + airtime);
                return "SUCCESS";
            } else {
                System.out.println("WNY");
                System.out.println("Transaction failed.");
                return "FAILED";
            }
        } catch (Exception e) {
            System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
    }
 
    /**
     * Updates the amount of airtime a phone number has in the database
     * 
     * @param phoneNumber the phone number to which the airtime is loaded
     * @param airtime the amount of airtime to be loaded
     * @return status of the process
     */
    @Override
    public String updateAirtime(String phoneNumber, int airtime) {
        try {
            String sql = "UPDATE " + table_name + " SET \"Airtime\" = ? WHERE \"Phone Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(sql);
            preparedStatement.setInt(1, airtime);
            preparedStatement.setString(2, phoneNumber);
 
            int rowsUpdated = preparedStatement.executeUpdate();
            if (rowsUpdated > 0) {
                System.out.println("Transaction successful. Airtime = " + airtime);
                return "SUCCESS";
            } else {
                System.out.println("Transaction failed.");
                return "FAILED";
            }
        } catch (Exception e) {
            System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
    }
}