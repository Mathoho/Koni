package VertNTemp;
import java.time.Duration;

import io.temporal.activity.ActivityOptions;
import io.temporal.workflow.Workflow;

public class WorkflowImplwrg{
    ActivityOptions options = ActivityOptions.newBuilder()
    .setStartToCloseTimeout(Duration.ofSeconds(60))
    .build();
   
    private final API1Interface activityAPI1 = Workflow.newActivityStub(API1Interface.class,options);
    private final API2Interface activityAPI2 = Workflow.newActivityStub(API2Interface.class,options);
    private final API3Interface activityAPI3 = Workflow.newActivityStub(API3Interface.class,options);
    
    
}
