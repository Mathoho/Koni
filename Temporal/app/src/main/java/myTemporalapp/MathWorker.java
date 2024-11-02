package myTemporalapp;

import io.temporal.client.WorkflowClient;
import io.temporal.serviceclient.WorkflowServiceStubs;
import io.temporal.worker.Worker;
import io.temporal.worker.WorkerFactory;

public class MathWorker {
    public static void main(String[] args) {

        /*  Get a Workflow service stub.
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();

        /*
        * Get a Workflow service client which can be used to start, Signal, and Query Workflow Executions.
        
        WorkflowClient client = WorkflowClient.newInstance(service);

        /*
        * Define the workflow factory. It is used to create workflow workers that poll specific Task Queues.
        
        WorkerFactory factory = WorkerFactory.newInstance(client);
        WorkerFactory factoryPayment = WorkerFactory.newInstance(client);

        /*
        * Define the workflow worker. Workflow workers listen to a defined task queue and process
        * workflows and activities.
        
        Worker worker = factory.newWorker(Shared.MATH_TASK_QUEUE);

        /*
        * Register our workflow implementation with the worker.
        * Workflow implementations must be known to the worker at runtime in
        * order to dispatch workflow tasks.
        
        worker.registerWorkflowImplementationTypes(MathWorkflowImpl.class);

        /*
        * Register our Activity Types with the Worker. Since Activities are stateless and thread-safe,
        * the Activity Type is a shared instance.
        
        worker.registerActivitiesImplementations(new MathActivitiesImpl());

        /*
        * Start all the workers registered for a specific task queue.
        * The started workers then start polling for workflows and activities.
        
        factory.start();
        */

        /*1 
        WorkflowServiceStubs transRevservice = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient transRevclient = WorkflowClient.newInstance(transRevservice);
        WorkerFactory transRevfactory = WorkerFactory.newInstance(transRevclient);
        Worker transRevworker = transRevfactory.newWorker(Shared.TRANSACTION_REVERSAL_TASK_QUEUE);
        transRevworker.registerWorkflowImplementationTypes(MathWorkflowImpl.class);
        transRevworker.registerActivitiesImplementations(new MathActivitiesImpl());
        transRevfactory.start();*/
        //api1Worker("TRANSACTION_REVERSAL_TASK_QUEUE");

        /*2 
        WorkflowServiceStubs purchaseAirtimeservice = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient purchaseAirtimeclient = WorkflowClient.newInstance(purchaseAirtimeservice);
        WorkerFactory purchaseAirtimefactory = WorkerFactory.newInstance(purchaseAirtimeclient);
        Worker purchaseAirtimeworker = purchaseAirtimefactory.newWorker(Shared.PURCHASE_AIRTIME_TASK_QUEUE);
        purchaseAirtimeworker.registerWorkflowImplementationTypes(API3Methods.class);
        purchaseAirtimeworker.registerActivitiesImplementations(new MathActivitiesImpl());
        purchaseAirtimefactory.start();*/
        
        //api3Worker("PURCHASE_DATA_TASK_QUEUE");

        /*3 
        WorkflowServiceStubs updateTransService = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient updateTransSclient = WorkflowClient.newInstance(updateTransService);
        WorkerFactory updateTransSfactory = WorkerFactory.newInstance(updateTransSclient);
        Worker updateTransSworker = updateTransSfactory.newWorker(Shared.UPDATE_TRANS_TASK_QUEUE);
        updateTransSworker.registerWorkflowImplementationTypes(API2Methods.class);
        updateTransSworker.registerActivitiesImplementations(new MathActivitiesImpl());
        updateTransSfactory.start();*/
       

        /*4
        WorkflowServiceStubs addTranservice = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient addTransclient = WorkflowClient.newInstance(addTranservice);
        WorkerFactory addTransfactory = WorkerFactory.newInstance(addTransclient);
        Worker addTransworker = addTransfactory.newWorker(Shared.ADD_TRANS_TASK_QUEUE);
        addTransworker.registerWorkflowImplementationTypes(API2Methods.class);
        addTransworker.registerActivitiesImplementations(new MathActivitiesImpl());
        addTransfactory.start();*/
       

        /*5 
        WorkflowServiceStubs transPayservice = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient transPayclient = WorkflowClient.newInstance(transPayservice);
        WorkerFactory transPayfactory = WorkerFactory.newInstance(transPayclient);
        Worker transPayworker = transPayfactory.newWorker(Shared.TRANSACTION_PAYMENT_TASK_QUEUE);
        transPayworker.registerWorkflowImplementationTypes(API1Methods.class);
        transPayworker.registerActivitiesImplementations(new MathActivitiesImpl());
        transPayfactory.start();*/
        api1Worker("TRANSACTION_PAYMENT_TASK_QUEUE"); //TODO a REVERSE ALSO

        api2Worker("ADD_TRANS_TASK_QUEUE");
        api3Worker("PURCHASE_AIRTIME_TASK_QUEUE"); //TODO or DATA here
        
        api2Worker("UPDATE_TRANS_TASK_QUEUE"); 

        

    }
    

    public static void api3Worker(String task) {
        //PURCHASE_DATA_TASK_QUEUE
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
        switch (task) {
            case "PURCHASE_AIRTIME_TASK_QUEUE":
                worker = factory.newWorker(Shared.PURCHASE_AIRTIME_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(purAirtmeWorkflowImpl.class);
                break;
            case "PURCHASE_DATA_TASK_QUEUE":
                worker = factory.newWorker(Shared.PURCHASE_DATA_TASK_QUEUE);
                worker.registerWorkflowImplementationTypes(purDataWorkflowImpl.class);
            default:
                break;
        }
        
        worker.registerActivitiesImplementations(new API3Methods());
        factory.start();
    }

    public static void api2Worker(String task){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
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
                break;
        }
        
        worker.registerActivitiesImplementations(new API2Methods());
        factory.start();
    }

    public static void api1Worker(String task){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        WorkerFactory factory = WorkerFactory.newInstance(client);
        Worker worker = null;
        switch (task) {
            case "TRANSACTION_REVERSAL_TASK_QUEUE":
                worker = factory.newWorker(Shared.TRANSACTION_REVERSAL_TASK_QUEUE);
                break;
            case "TRANSACTION_PAYMENT_TASK_QUEUE":
                worker = factory.newWorker(Shared.TRANSACTION_PAYMENT_TASK_QUEUE);
                break;
            default: 
                break;
        }
        
        worker.registerWorkflowImplementationTypes(TransWorkflowImpl.class);
        worker.registerActivitiesImplementations(new API1Methods());
        factory.start();
    }
}

