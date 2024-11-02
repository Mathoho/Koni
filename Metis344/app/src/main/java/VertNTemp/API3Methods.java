package VertNTemp;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import io.vertx.core.json.JsonObject;
 
/**
 * Contains all product functions
 * 
 * @author Stanley Shikwambi
 * @author Jano Beukes
 */
public class API3Methods implements API3Interface{
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
 
            } else {
                System.out.println("Connection to API3 Database failed");
            }
        } catch (Exception e) {
            //System.out.println(e);
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
            //System.out.println("Error performing transaction: " + e.getMessage());
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
                System.out.println("Transaction failed.");
                return "FAILED";
            }
        } catch (Exception e) {
            //System.out.println("Error performing transaction: " + e.getMessage());
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
            //System.out.println("Error performing transaction: " + e.getMessage());
            return "ERROR";
        }
    }
    //update user phone number, get the user balance for airtime, data
    public JsonObject getDataBalance(String phoneNumber){
        JsonObject dataJsonObject = new JsonObject();
        try {
            String query = "SELECT \"Data\" from " +table_name + " WHERE \"Phone Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(query);
            preparedStatement.setString(1, phoneNumber);
            ResultSet rs = preparedStatement.executeQuery();
            rs.next();
            int data = rs.getInt("Data");
            dataJsonObject
                .put("Data",data);
            return dataJsonObject;
        } catch (Exception e) {
            dataJsonObject
                .put("Error","Failed getting the user\'s Data balance");
            return dataJsonObject;
        }
    }

    public JsonObject getAirtimeBalance(String phoneNumber){
        JsonObject airtimeJsonObject = new JsonObject();
        try {
            String query = "SELECT \"Airtime\" from " +table_name + " WHERE \"Phone Number\" = ?";
            PreparedStatement preparedStatement = conn.prepareStatement(query);
            preparedStatement.setString(1, phoneNumber);
            ResultSet rs = preparedStatement.executeQuery();
            rs.next();
            int airtime = rs.getInt("Airtime");
            airtimeJsonObject
                .put("Airtime",airtime);
            return airtimeJsonObject;
        } catch (Exception e) {
            airtimeJsonObject
                .put("Error","Failed getting the user\'s Data balance");
            return airtimeJsonObject;
        }
    }

       public static void main(String[] args) {
    System.out.println("in the main method now");
    API3Methods db3 = new API3Methods();
        db3.connect_to_db("api3database", "konanani", "123456");
        JsonObject val = db3.getDataBalance("0849871234");
        System.out.println("++++++++++++++++++++++++++++++++++++");
        System.out.println(val.toString());
        System.out.println("++++++++++++++++++++++++++++++++");
        JsonObject val2 = db3.getAirtimeBalance("0732975034");
        System.out.println(val2.toString());
   }
}