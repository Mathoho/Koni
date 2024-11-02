package myTemporalapp;

import io.temporal.client.WorkflowClient;
import io.temporal.client.WorkflowOptions;
import io.temporal.client.WorkflowStub;
import io.temporal.serviceclient.WorkflowServiceStubs;

public class InitiateMath {
    public static void main(String[] args) {
        // // This gRPC stubs wrapper talks to the local docker instance of the Temporal service.
        // WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
         
        // // Define our workflow unique id
        //  final String WORKFLOW_ID = "MathWorkflowID";
         
        //  /*
        //  * Set Workflow options such as WorkflowId and Task Queue so the worker knows where to list and which workflows to execute.
        //  */
        // WorkflowOptions options = WorkflowOptions.newBuilder()
        //             .setWorkflowId(WORKFLOW_ID)
        //             .setTaskQueue(Shared.MATH_TASK_QUEUE)
        //             .build();

        // // WorkflowClient can be used to start, signal, query, cancel, and terminate Workflows.
        // WorkflowClient client = WorkflowClient.newInstance(service);

       

       

        // // Create the workflow client stub. It is used to start our workflow execution.
        // MathWorkflow workflow = client.newWorkflowStub(MathWorkflow.class, options);

        // /*
        //  * Execute our workflow and wait for it to complete. The call to our getGreeting method is
        //  * synchronous.
        //  * 
        //  * Replace the parameter "World" in the call to getGreeting() with your name.
        //  */
        // //String greeting = workflow.getGreeting("World");
        // int sum = workflow.Addition(1,2);
        // String workflowId = WorkflowStub.fromTyped(workflow).getExecution().getWorkflowId();
        // // Display workflow execution results
        // System.out.println(workflowId + " " + sum);
        // System.exit(0);
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();

        System.out.println("No vuwa hani");
    }
}
