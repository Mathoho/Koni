package myTemporalapp;

import java.sql.Connection;
import java.time.Duration;

import io.temporal.activity.ActivityOptions;
import io.temporal.workflow.Workflow;

//public class MathWorkflowImpl implements MathWorkflow{
    public class MathWorkflowImplwrg{
    ActivityOptions options = ActivityOptions.newBuilder()
    .setStartToCloseTimeout(Duration.ofSeconds(60))
    .build();
    //private final MathActivities activity = Workflow.newActivityStub(MathActivities.class,options);
    private final API1MethodsInfrc activityAPI1 = Workflow.newActivityStub(API1MethodsInfrc.class,options);
    private final API2MethodsInfrc activityAPI2 = Workflow.newActivityStub(API2MethodsInfrc.class,options);
    private final API3MethodsInfrc activityAPI3 = Workflow.newActivityStub(API3MethodsInfrc.class,options);
    public static void ntg(){
        System.out.println("i am nothing");
    }

   


    


   


    


    
}
