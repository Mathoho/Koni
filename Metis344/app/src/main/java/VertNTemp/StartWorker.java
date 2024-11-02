
package VertNTemp;
import java.io.BufferedReader;
import java.io.FileReader;

import io.temporal.client.WorkflowClient;
import io.temporal.serviceclient.WorkflowServiceStubs;
import io.temporal.worker.Worker;
import io.temporal.worker.WorkerFactory;

public class StartWorker {
    public static void main(String[] args) {

       
        String taskApi1 = "TRANSACTION_PAYMENT_TASK_QUEUE";
        api1Worker(taskApi1); 
        String taskAPi3 = "PURCHASE_AIRTIME_TASK_QUEUE";
        String line;
        try {
            //FileReader rstFile = new FileReader("./INPUT/resultAirtime.txt");
            FileReader rstFile = new FileReader("./INPUT/resultData.txt");
            //FileReader rstFile = new FileReader("./INPUT/resultReverse.txt");
            //FileReader rstFile = new FileReader("./INPUT/resultInscf.txt");
            BufferedReader bufferedReader = new BufferedReader(rstFile);

            // Read each line in the file until the end of the file is reached
            while ((line = bufferedReader.readLine()) != null) {
                String[] arr = line.split(":");
                String key = arr[0].strip();
                if (key.equals("productType")) {
                    String type = arr[1].strip();
                    if (type.equals("data")) {
                        taskAPi3 = "PURCHASE_DATA_TASK_QUEUE";
                        api3Worker(taskAPi3); 
                    }else if (type.equals("airtime")) {
                        api3Worker(taskAPi3); 
                    }
                } else if (key.equals("REVERSAL")){
                    taskApi1 = "TRANSACTION_REVERSAL_TASK_QUEUE";
                    api1Worker(taskApi1); 
                }
               
            }
            bufferedReader.close();
        } catch (Exception e) {
            System.out.println(e);
        } 
         // Start workers for different task queues and workflow implementations.
        api2Worker("ADD_TRANS_TASK_QUEUE"); 
        api2Worker("UPDATE_TRANS_TASK_QUEUE"); 
        
    }
    

    /**
     * Start a worker for API3 tasks.
     *
     * @param task The task queue to listen to.
     */
    public static void api3Worker(String task) {
        //Get a Workflow service stub.
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
        //assigning worker to a specific task and then registering the worker
        switch (task) {
            case "PURCHASE_AIRTIME_TASK_QUEUE":
                worker = factory.newWorker(Shared.PURCHASE_AIRTIME_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(purAirtmeWorkflowImpl.class);
                worker.registerActivitiesImplementations(new API3Methods());
                factory.start();
                break;
            case "PURCHASE_DATA_TASK_QUEUE":
                worker = factory.newWorker(Shared.PURCHASE_DATA_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(purDataWorkflowImpl.class);
                worker.registerActivitiesImplementations(new API3Methods());
                factory.start();
            default:
                return;
        }
       
    }

    /**
     * Start a worker for API2 tasks.
     *
     * @param task The task queue to listen to.
     */
    public static void api2Worker(String task){
        //Get a Workflow service stub.
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
        //assigning worker to a specific task and then registering the worker
        switch (task) {
            case "ADD_TRANS_TASK_QUEUE":
                worker = factory.newWorker(Shared.ADD_TRANS_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(AddTransWorflowImpl.class);
                break;
            case "UPDATE_TRANS_TASK_QUEUE":
                worker = factory.newWorker(Shared.UPDATE_TRANS_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(updateTransworkflowImpl.class);
                break;
            default:
                return;
        }
        worker.registerActivitiesImplementations(new API2Methods());
        factory.start();
    }

    /**
     * Start a worker for API1 tasks.
     *
     * @param task The task queue to listen to.
     */
    public static void api1Worker(String task){
        //Get a Workflow service stub.
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
        //assigning worker to a specific task
        switch (task) {
            case "TRANSACTION_REVERSAL_TASK_QUEUE":
                worker = factory.newWorker(Shared.TRANSACTION_REVERSAL_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(RevTransWrkFlwImpl.class);
                break;
            case "TRANSACTION_PAYMENT_TASK_QUEUE":
                worker = factory.newWorker(Shared.TRANSACTION_PAYMENT_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(TransWorkflowImpl.class);
                break;
            default: 
                return;
        }
        //registering the worker
       
        worker.registerActivitiesImplementations(new API1Methods());
        factory.start();
    }
}

