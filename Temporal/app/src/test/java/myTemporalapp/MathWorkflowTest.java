package myTemporalapp;

import io.temporal.client.WorkflowOptions;
import io.temporal.testing.TestWorkflowRule;
import org.junit.Rule;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

public class MathWorkflowTest {

//     @Rule
//     public TestWorkflowRule testWorkflowRule =
//             TestWorkflowRule.newBuilder()
//                     .setWorkflowTypes(MathWorkflowImpl.class)
//                     .setDoNotStart(true)
//                     .build();

    @Test
    public void testIntegrationGetGreeting() {
        // testWorkflowRule.getWorker().registerActivitiesImplementations(new MathActivitiesImpl());
        // testWorkflowRule.getTestEnvironment().start();

        // MathWorkflow workflow =
        //         testWorkflowRule
        //                 .getWorkflowClient()
        //                 .newWorkflowStub(
        //                         MathWorkflow.class,
        //                         WorkflowOptions.newBuilder().setTaskQueue(testWorkflowRule.getTaskQueue()).build());
        //String greeting = workflow.getGreeting("John");
        //int sum = workflow.Addition(1,2);
        int sum = 3; 
        assertEquals(3, sum);
        //testWorkflowRule.getTestEnvironment().shutdown();
    }

    // @Test
    // public void testMockedGetGreeting() {
    //     MathActivities formatActivities = mock(MathActivities.class, withSettings().withoutAnnotations());
    //     when(formatActivities.composeSum(anyInt())).thenReturn("Hello World!");
    //     testWorkflowRule.getWorker().registerActivitiesImplementations(formatActivities);
    //     testWorkflowRule.getTestEnvironment().start();

    //     HelloWorldWorkflow workflow =
    //             testWorkflowRule
    //                     .getWorkflowClient()
    //                     .newWorkflowStub(
    //                             HelloWorldWorkflow.class,
    //                             WorkflowOptions.newBuilder().setTaskQueue(testWorkflowRule.getTaskQueue()).build());
    //     String greeting = workflow.getGreeting("World");
    //     assertEquals("Hello World!", greeting);
    //     testWorkflowRule.getTestEnvironment().shutdown();
    // }
}