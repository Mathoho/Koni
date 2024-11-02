package VertNTemp;
import java.util.concurrent.CompletableFuture;

import io.temporal.api.common.v1.WorkflowExecution;
import io.temporal.client.WorkflowClient;
import io.temporal.client.WorkflowOptions;
import io.temporal.serviceclient.WorkflowServiceStubs;

/**
 * A class containing methods for interacting with Temporal workflows.
 */
public class TemporalMethods {
    
    /**
     * Updates a transaction using a Temporal workflow.
     *
     * @param date       The date of the transaction update.
     * @param time       The time of the transaction update.
     * @param msg        The message for the transaction update.
     * @param PAN        The PAN (Primary Account Number).
     * @param api2       An instance of the API2Methods class.
     * @return True if the transaction update is successful, otherwise false.
     */
    public boolean updateTransTemp(String date, String time, String msg, String PAN,API2Methods api2){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN "+ PAN+ " UPDATED_TRANS: at time: "+ time;
        // Configure the options for the workflow.
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.UPDATE_TRANS_TASK_QUEUE)
                    .build();

        //Starting the workflow
        updateTransworkflow workflow = client.newWorkflowStub(updateTransworkflow.class, options);
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::updateTrans_temp, date,time,msg);
        System.out.println(wrkflwexc.getWorkflowId() + " updated the trans  ");
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());
        boolean rslt = api2.updateTransStatus(date, time, msg);
        return rslt;
    }

    /**
     * Purchases airtime using a Temporal workflow.
     *
     * @param phoneNumber     The phone number for which to purchase airtime.
     * @param date            The date of the airtime purchase.
     * @param time            The time of the airtime purchase.
     * @param amountProduct   The amount of airtime to purchase.
     * @param PAN             The PAN (Primary Account Number).
     * @param api3            An instance of the API3Methods class.
     * @return A string indicating the result of the airtime purchase.
     */
   //TODO public CompletableFuture<String> purAirtmTemp(String phoneNumber, String date, String time, int amountProduct, String PAN,API3Methods api3){
        //return CompletableFuture.supplyAsync(() -> {
    public String purAirtmTemp(String phoneNumber, String date, String time, int amountProduct, String PAN,API3Methods api3){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN +" PURCHASE AIRTIME at time: "+time;
        
        // Configure the options for the airtime purchase workflow.
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.PURCHASE_AIRTIME_TASK_QUEUE)
                    .build();

        //Starting the workflow
        purAirtmeWorkflow workflow = client.newWorkflowStub(purAirtmeWorkflow.class, options);
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::purchaseAirtime_temp, phoneNumber,date,time,amountProduct);
        System.out.println(wrkflwexc.getWorkflowId() + " purchased airtime of amount " + amountProduct);
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());

        String rslt = api3.purchaseAirtime(phoneNumber, date, time, amountProduct);
        return rslt;
       // });
    }

    /**
     * Adds a transaction using a Temporal workflow.
     *
     * @param userID     The user ID associated with the transaction.
     * @param amount     The amount of the transaction.
     * @param date       The date of the transaction.
     * @param time       The time of the transaction.
     * @param PAN        The PAN (Primary Account Number).
     * @param api2       An instance of the API2Methods class.
     * @return A string indicating the result of the transaction addition.
     */
    //TODOD public CompletableFuture<String> addTransTemp(int userID, int amount, String date, String time, String PAN,API2Methods api2) {
       //TODO return CompletableFuture.supplyAsync(() -> {
    public String addTransTemp(int userID, int amount, String date, String time, String PAN,API2Methods api2) {
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN +" adding trans at TIME: "+time;
        
        // Configure the options for the add transaction workflow.
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.ADD_TRANS_TASK_QUEUE)
                    .build();

        //Starting the workflow
        AddTransWorflow workflow = client.newWorkflowStub(AddTransWorflow.class, options);
        WorkflowExecution addTranswe = WorkflowClient.start(workflow::AddTrans_temp, userID, amount,date,time);
        System.out.println(addTranswe.getWorkflowId() + " added trans of amount to database " + amount);
        System.out.printf("\nWorkflowID: %s RunID: %s", addTranswe.getWorkflowId(), addTranswe.getRunId());
        String rslt = api2.addTrans(userID, amount, date, time);
        return rslt;
        //});
    }

    /**
     * Initiates a financial transaction using a Temporal workflow.
     *
     * @param PAN      The PAN (Primary Account Number).
     * @param amount   The amount of the transaction.
     * @param msg      The message for the transaction (e.g., "PAYMENT" or "REVERSAL").
     * @param time     The time of the transaction.
     * @param api1     An instance of the API1Methods class.
     * @return A string indicating the result of the financial transaction.
     */
    //public CompletableFuture<String> transactionTemp(String PAN, int amount, String msg,String time,API1Methods api1){
     //   return CompletableFuture.supplyAsync(() -> {
    public String transactionTemp(String PAN, int amount, String msg,String time,API1Methods api1){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+PAN+ " did a Trans " + msg + " of amount: " + amount+ " at time: "+time;
        // Configure the options for the payment or reversal workflow.
        WorkflowOptions options = null;
        switch (msg) {
            case "PAYMENT":
            System.out.println("making payment");
            options = WorkflowOptions.newBuilder()
            .setWorkflowId(WORKFLOW_ID)
            .setTaskQueue(Shared.TRANSACTION_PAYMENT_TASK_QUEUE)
            .build();
            break;
            case "REVERSAL":
            System.out.println("making payment reversal");
                options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.TRANSACTION_REVERSAL_TASK_QUEUE)
                    .build();
                break;
            default:
                return "";
        }
        
        TransWorkflow workflow = client.newWorkflowStub(TransWorkflow.class, options);
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::Transaction_temp, PAN, amount, msg);
        System.out.println(wrkflwexc.getWorkflowId() + " did a Trans " + msg + " of amount: " + amount);
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());
        
        String rslt = api1.transaction(PAN, amount, msg);

        return rslt;
       // });
    }

    /**
     * Purchases data using a Temporal workflow.
     *
     * @param phoneNumber     The phone number for which to purchase data.
     * @param date            The date of the data purchase.
     * @param time            The time of the data purchase.
     * @param amountProduct   The amount of data to purchase.
     * @param PAN             The PAN (Primary Account Number).
     * @return A string indicating the result of the data purchase.
     */
   //todo  public CompletableFuture<String> purDataTemp(String phoneNumber, String date, String time, int amountProduct, String PAN,API3Methods api3){
    //    return CompletableFuture.supplyAsync(() -> {
    public String purDataTemp(String phoneNumber, String date, String time, int amountProduct, String PAN,API3Methods api3){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN+" PURCHASED DATA at time: "+ time;
        
        // Configure the options for the data purchase workflow.
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.PURCHASE_DATA_TASK_QUEUE)
                    .build();

        //Starting the workflow
        purDataWorkflow workflow = client.newWorkflowStub(purDataWorkflow.class, options);
        WorkflowExecution purDatamWe = WorkflowClient.start(workflow::purchaseData_temp, phoneNumber,date,time,amountProduct);

        System.out.println(purDatamWe.getWorkflowId() +" of size" + amountProduct);
        System.out.printf("\nWorkflowID: %s RunID: %s", purDatamWe.getWorkflowId(), purDatamWe.getRunId());
        String rslt = api3.purchaseData(phoneNumber, date, time, amountProduct);
        return rslt;
        //});
    }
}
