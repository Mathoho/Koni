package myTemporalapp;

import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface MathActivities {

    @ActivityMethod
    int composeSum(int sum);
    // int composeSub(int sum);
    // int composeDiv(int sum);
    // int composeMul(int sum);
    //TODO did not implement since it is the APIMethods
}
