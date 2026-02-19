/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Robot:        Teamwork                                             */
/*    Created:      24/1/2026, 08:59:47 AM                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;

// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor MotorPinMotorA = motor(PORT6, false);
motor MotorPinMotorB = motor(PORT12, true);
motor_group MotorPin = motor_group(MotorPinMotorA, MotorPinMotorB);

motor MotorBeamMotorA = motor(PORT8, true);
motor MotorBeamMotorB = motor(PORT9, false);
motor_group MotorBeam = motor_group(MotorBeamMotorA, MotorBeamMotorB);

pneumatic Pneumatic_font = pneumatic(PORT2);
pneumatic Pneumatic_back = pneumatic(PORT3);

distance Distance = distance(PORT4);

touchled TouchLED = touchled(PORT10);

motor MotorLeft = motor(PORT5, true);
motor MotorRight = motor(PORT11, false);

//================ ENUM =================

enum class DriveDirection
{
    FORWARD,
    BACKWARD
};

enum class BeamState
{
    ON,
    OFF
};

enum class PinState
{
    ON,
    OFF
};

enum class ButtonStage
{
    IDLE,
    STEP1,
    STEP2
};

//================ STRUCT =================

struct RobotState
{
    DriveDirection driveDir;
    BeamState beamState;
    PinState pinState;

    bool stop = false;
    bool isBusy = false;
    bool MakeY = false;
    bool Eup = false;
    bool EDown = false;
    bool Go = false;
    bool NewDowm = false;

    // double nowTime = 0;
    double Kspeed = 1.05;
    double speedB = 0.55;
};

// controller state
struct ControllerState
{
    ButtonStage Lup = ButtonStage::IDLE;
    ButtonStage Ldown = ButtonStage::IDLE;
    ButtonStage Rup = ButtonStage::IDLE;
    ButtonStage Rdown = ButtonStage::IDLE;
};

//================ GLOBAL OBJECT =================

RobotState robot;
ControllerState controllerState;

// generating and setting random seed
void initializeRandomSeed()
{
    wait(100, msec);
    double xAxis = BrainInertial.acceleration(xaxis) * 1000;
    double yAxis = BrainInertial.acceleration(yaxis) * 1000;
    double zAxis = BrainInertial.acceleration(zaxis) * 1000;
    // Combine these values into a single integer
    int seed = int(
        xAxis + yAxis + zAxis);
    // Set the seed
    srand(seed);
}

void vexcodeInit()
{

    // Initializing random seed.
    initializeRandomSeed();
}

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

// User defined function
void Spin_Robot();
// User defined function
void Place_beam();
// User defined function
void Grab_Beam_up();
// User defined function
void Lup_Init();
// User defined function
void Drop_down_beam();
// User defined function
void Ldown_Init();
// User defined function
void Rdown_Init();
// User defined function
void Grab_then_up();
// User defined function
void Rup_Init();
// User defined function
void Drop_down();
// User defined function
void Drop_down_Grab_Up();
// User defined function
void EUp_pressed();
void Extend_Pneumatic_Beam();
void Retract_Pneumatic_Beam();
void Extend_joint_pin();
void Retract_joint_pin();
void Retract_2Pneumatic_Pin();
void Extend_2Pneumatic_Pin();
void Ajpin();

int Screen_precision = 0, Console_precision = 0;

// float Rup, Rdown, Lup, Ldown, Kspeed, timee = 0.0;

// bool front_back, pin, beamOn, isBusy, stop, stand_off_up, start_stand_off_up, MakeY, Eup;

// User defined function
void Place_beam()
{
    robot.stop = true;
    robot.driveDir = DriveDirection::FORWARD;
    MotorBeam.setStopping(hold);
    MotorBeam.stop();
    if (robot.MakeY)
    {
        MotorBeam.setVelocity(45.0, percent);
        MotorBeam.spinFor(forward, 120.0, degrees, true);
    }
    else
    {
        MotorBeam.setVelocity(45.0, percent);
        MotorLeft.setStopping(hold);
        MotorRight.setStopping(hold);
        MotorLeft.stop();
        MotorRight.stop();
        wait(0.1, seconds);
        MotorBeam.spinFor(forward, 50.0, degrees, true);
    }
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    Retract_Pneumatic_Beam();
    wait(200, msec);
    MotorLeft.setVelocity(85.0, percent);
    MotorRight.setVelocity(85.0, percent);
    MotorLeft.spinFor(forward, 500.0, degrees, false);
    MotorRight.spinFor(forward, 500.0, degrees, false);
    wait(0.55, seconds);
    robot.stop = false;
    robot.beamState = BeamState::OFF;
    // robot.speedB = 0.55;
    Drop_down_beam();
    robot.isBusy = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("PBeam_Done: %.2f", robot.nowTime);
}

// User defined function
void Grab_Beam_up()
{
    MotorBeam.setStopping(coast);
    MotorBeam.stop();
    wait(0.5, seconds);
    MotorBeam.spinFor(reverse, 300.0, degrees, false);
    robot.isBusy = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("GBeam_Done: %.2f", robot.nowTime);
}

// User defined function
void Lup_Init()
{
    Brain.Timer.reset();
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Drop_down_beam()
{
    controllerState.Lup = ButtonStage::IDLE;
    robot.stop = false;
    MotorBeam.setStopping(hold);
    robot.beamState == BeamState::OFF ? MotorBeam.setVelocity(100, percent) : MotorBeam.setVelocity(45, percent);
    MotorBeam.spin(forward);
    MotorBeam.setMaxTorque(100, percent);
    wait(100, msec);
    while (MotorBeam.velocity(vex::velocityUnits::pct) != 0)
    {
        MotorBeam.spin(forward);
        wait(10, msec);
    }
    MotorBeam.setStopping(brake);
    MotorBeam.setMaxTorque(0, percent);
    MotorBeam.stop();
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    robot.isBusy = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("DBeam_Done: %.2f", robot.nowTime);
}

// User defined function
void Ldown_Init()
{
    Brain.Timer.reset();
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
}

// User defined function
void Rdown_Init()
{
    Brain.Timer.reset();
    MotorLeft.setStopping(hold);
    MotorRight.setStopping(hold);
    MotorPin.setVelocity(50.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
}

// User defined function
void Grab_then_up()
{
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    Extend_2Pneumatic_Pin();
    wait(0.1, seconds);
    MotorPin.spinToPosition(190.0, degrees, false);
    wait(0.3, seconds);
    Extend_joint_pin();
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("GPin_Done: %.2f", robot.nowTime);
}
// User defined function
void Rup_Init()
{
    Brain.Timer.reset();
    MotorPin.setVelocity(100.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorPin.setStopping(hold);
}

// User defined function
void Drop_down()
{
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    MotorPin.spinToPosition(-50.0, degrees, false);
    wait(0.1, seconds);
    Retract_joint_pin();
    MotorPin.spinToPosition(-170.0, degrees, false);
    wait(0.20, seconds);
    while (MotorPin.velocity(vex::velocityUnits::pct) > 5)
    {
        MotorPin.spin(reverse);
        wait(1, msec);
    }
    MotorPin.setStopping(coast);
    MotorPin.stop();
    wait(100, msec);
    Retract_2Pneumatic_Pin();
    wait(150, msec);
    // Extend_2Pneumatic_Pin();
    robot.Eup = true;
    robot.pinState = PinState::OFF;
    controllerState.Rup = ButtonStage::IDLE;
    robot.NewDowm = true;
    // MotorPin.setPosition(0.0, degrees);
    // MotorPin.spinFor(forward, 70, degrees, true);
}

// User defined function
void Drop_down_Grab_Up()
{
    robot.stop = true;
    // wait(0.1, seconds);
    // robot.stop = false;
    MotorLeft.stop();
    MotorRight.stop();
    MotorPin.resetPosition();
    MotorPin.setPosition(0.0, degrees);
    MotorPin.setStopping(hold);
    MotorPin.setVelocity(60.0, percent);
    MotorLeft.setVelocity(100.0, percent);
    MotorRight.setVelocity(100.0, percent);
    // MotorPin.spinToPosition(-50.0, degrees, false);
    MotorPin.spin(reverse);
    wait(0.15, seconds);
    while (MotorPin.velocity(vex::velocityUnits::pct) > 1)
    {
        MotorPin.spin(reverse);
        wait(1, msec);
    }
    MotorLeft.setStopping(brake);
    MotorRight.setStopping(brake);
    MotorLeft.stop();
    MotorRight.stop();
    wait(0.1, seconds);
    Retract_joint_pin();
    Retract_2Pneumatic_Pin();
    // MotorPin.spinToPosition(-170.0, degrees, false);
    MotorPin.spin(reverse);
    // MotorLeft.spinFor(reverse, 20.0, degrees, false);
    // MotorRight.spinFor(reverse, 20.0, degrees, true);
    robot.stop = true;
    wait(0.15, seconds);
    // Retract_2Pneumatic_Pin();
    // while (MotorPin.velocity(vex::velocityUnits::pct) > 5)
    // {
    //     MotorPin.spin(reverse);
    //     wait(1, msec);
    // }
    MotorLeft.spinFor(forward, 300.0, degrees, false);
    MotorRight.spinFor(forward, 300.0, degrees, false);
    wait(0.2, seconds);
    MotorPin.stop(hold);
    Extend_2Pneumatic_Pin();
    wait(0.1, seconds);
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    MotorLeft.stop();
    MotorRight.stop();
    robot.stop = false;
    MotorPin.setVelocity(100, percent);
    MotorPin.setMaxTorque(100, pct);
    MotorPin.resetPosition();
    MotorPin.spinToPosition(210.0, degrees, false);
    wait(0.2, seconds);
    Extend_joint_pin();
    robot.isBusy = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("D&GPin_Done: %.2f", robot.nowTime);
}
// User defined function
void Spin_Robot()
{
    robot.stop = true;
    MotorLeft.stop();
    MotorRight.stop();
    MotorPin.setStopping(coast);
    robot.driveDir = DriveDirection::BACKWARD;
    if (robot.EDown == true)
    {
        /* pass */
    }
    else
    {
        if (controllerState.Lup == ButtonStage::STEP1 || controllerState.Lup == ButtonStage::STEP2)
        {
            Drop_down();
            Retract_2Pneumatic_Pin();
            controllerState.Rup = ButtonStage::IDLE;
            controllerState.Rdown = ButtonStage::IDLE;
        }
        else
        {
            MotorPin.setStopping(coast);
        }
    }
    MotorRight.setStopping(hold);
    MotorLeft.setStopping(hold);
    MotorLeft.setVelocity(40.0, percent);
    MotorRight.setVelocity(40.0, percent);
    MotorLeft.spinFor(reverse, 170.0, degrees, false);
    MotorRight.spinFor(reverse, 170.0, degrees, true);
    MotorLeft.spinFor(forward, 270.0, degrees, false);
    MotorRight.spinFor(reverse, 270.0, degrees, true);
    MotorLeft.setVelocity(60.0, percent);
    MotorRight.setVelocity(60.0, percent);
    MotorRight.setStopping(coast);
    MotorLeft.setStopping(coast);
    MotorLeft.spinFor(reverse, 100.0, degrees, false);
    MotorRight.spinFor(reverse, 100.0, degrees, true);
    robot.pinState = PinState::OFF;
    // Brain.Screen.clearLine(4, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(4, 1);
    // Brain.Screen.print("Spin_Robot_Done: %.2f", robot.nowTime);
}

void standoff()
{
    if (robot.isBusy == false)
    {
        Brain.Timer.reset();
        robot.isBusy = true;
        MotorBeam.setMaxTorque(100, percent);
        MotorBeam.setVelocity(100.0, percent);
        if (Controller.AxisC.position() > 90.0)
        {
            robot.stop = true;
            wait(0.1, sec);
            robot.stop = true;
            MotorLeft.spin(forward, 50.0, percent);
            MotorRight.spin(forward, 50.0, percent);
            Retract_2Pneumatic_Pin();
            MotorPin.spin(reverse);
            wait(500, msec);
            MotorBeam.spinFor(reverse, 600.0, degrees, false);
            // Retract_2Pneumatic_Pin();
            MotorPin.stop(coast);
            robot.pinState = PinState::OFF;
            Retract_joint_pin();
            MotorBeam.setStopping(hold);
            Spin_Robot();
            MotorPin.stop();
            controllerState.Lup = ButtonStage::STEP1;
            controllerState.Rup = ButtonStage::IDLE;
            robot.MakeY = false;
            robot.stop = false;
            // Brain.Screen.clearLine(3, 1);
            // robot.nowTime = Brain.Timer.value();
            // Brain.Screen.setCursor(3, 1);
            // Brain.Screen.print("ConC_Done: %.2f", robot.nowTime);
        }
        else if (Controller.AxisC.position() < -90.0)
        {
            // Retract_joint_pin();
            MotorPin.setVelocity(100, pct);
            MotorPin.spinToPosition(320, degrees, false);
            wait(100, msec);
            Extend_joint_pin();
            // Brain.Screen.clearLine(3, 1);
            // robot.nowTime = Brain.Timer.value();
            // Brain.Screen.setCursor(3, 1);
            // Brain.Screen.print("ConC_Done: %.2f", robot.nowTime);
        }
        robot.isBusy = false;
    }
}
void Ajpin()
{
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    TouchLED.setColor(green);
    while (MotorLeft.velocity(vex::velocityUnits::pct) && MotorRight.velocity(vex::velocityUnits::pct) > 0)
    {
        MotorLeft.stop();
        MotorRight.stop();
        wait(10, msec);
    }
    MotorLeft.setVelocity(45, percent);
    MotorRight.setVelocity(45, percent);
    MotorLeft.spinFor(forward, 100.5, degrees, false);
    MotorRight.spinFor(forward, 100.5, degrees, true);
    MotorLeft.setStopping(hold);
    MotorRight.setStopping(hold);
    MotorLeft.stop();
    MotorRight.stop();
    wait(200, msec);
}

void Pin_On_the_sidelines()
{
    if (robot.Eup)
    {
        MotorPin.resetPosition();
        // MotorPin.setPosition(0.0, degrees);
        MotorPin.spinFor(100.0, degrees, false);
        robot.Eup = false;
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("robot.Eup_Done: %.2f", robot.nowTime);
    }
    else
    {
        Brain.Timer.reset();
        robot.stop = true;
        MotorLeft.setVelocity(100, percent);
        MotorRight.setVelocity(100, percent);
        MotorPin.resetPosition();
        MotorPin.setPosition(0.0, degrees);
        Extend_2Pneumatic_Pin();
        wait(0.2, seconds);
        MotorLeft.spinFor(reverse, 130.0, degrees, false);
        MotorRight.spinFor(reverse, 130.0, degrees, false);
        MotorPin.spinToPosition(80.0, degrees, true);
        Extend_joint_pin();
        wait(0.2, seconds);
        robot.stop = false;
        //----
        wait(0.2, seconds);
        MotorPin.setVelocity(100, percent);
        MotorPin.setMaxTorque(1, pct);
        MotorPin.spin(reverse);
        Retract_joint_pin();
        wait(250, msec);
        robot.isBusy = true;
        while (MotorPin.velocity(vex::velocityUnits::pct) != 0)
        {
            MotorPin.spin(reverse);
            wait(10, msec);
        }
        // Retract_2Pneumatic_Pin();
        MotorPin.setStopping(coast);
        MotorPin.stop(coast);
        MotorLeft.setStopping(coast);
        MotorRight.setStopping(coast);
        MotorPin.setMaxTorque(100, pct);
        wait(0.1, sec);
        Grab_then_up();
        // MotorPin.setPosition(0.0, degrees);
        // MotorPin.resetPosition();
        //----
        controllerState.Rup = ButtonStage::STEP1;
        controllerState.Rdown = ButtonStage::IDLE;
        robot.pinState = PinState::OFF;
        robot.isBusy = false;
        robot.Eup = true;
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("EUp_Done: %.2f", robot.nowTime);
    }
}

void Pin_on_go()
{
    if (robot.isBusy == false)
    {
        if (Controller.AxisD.position() >= 90.0)
        {
            // robot.Eup = true;
            Pneumatic_font.retract(cylinder1);
            Retract_joint_pin();
            MotorPin.resetPosition();
            MotorPin.setVelocity(75, percent);
            // wait(350, msec);
            // Robot NO2 = 125.0
            // Robot NO3 = 100.0
            MotorPin.resetPosition();
            MotorPin.spinFor(reverse, 95.0, degrees, true);
            MotorPin.setVelocity(100, percent);
        }
    }
    robot.stop = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("ConD_Done: %.2f", robot.nowTime);
}

void flip()
{
    Brain.Timer.reset();
    MotorPin.setVelocity(100.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    if (robot.isBusy == false && (controllerState.Rup == ButtonStage::STEP1 || controllerState.Rup == ButtonStage::STEP2))
    {
        robot.isBusy = true;
        wait(100, msec);
        robot.isBusy = true;
        MotorPin.resetPosition();
        MotorPin.setPosition(0.0, degrees);
        MotorBeam.stop(coast);
        Retract_joint_pin();
        MotorPin.spinFor(forward, 700.0, degrees, false);
        MotorPin.setVelocity(100, percent);
        MotorPin.setMaxTorque(100, percent);
        MotorPin.spin(forward);
        wait(1400, msec);
        // while (MotorPin.velocity(vex::velocityUnits::pct) > 5)
        // {
        //     MotorPin.spin(forward);
        //     wait(1, msec);
        // }

        Retract_2Pneumatic_Pin();
        Retract_2Pneumatic_Pin();
        Retract_2Pneumatic_Pin();

        MotorPin.setVelocity(100, percent);

        MotorPin.spinFor(reverse, 1500.0, degrees, false);
        wait(1, seconds);
        MotorPin.setStopping(coast);
        MotorPin.stop();

        controllerState.Rup = ButtonStage::IDLE;
        robot.pinState = PinState::OFF;
        wait(0.1, seconds);
        robot.isBusy = false;

        robot.stop = false;
        MotorBeam.spinFor(reverse, 100.0, degrees, false);
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("R3_Done: %.2f", robot.nowTime);
    }
}

// "when Controller ButtonLUp pressed" hat block
void LUp_pressed()
{
    if (robot.isBusy == false)
    {
        Lup_Init();
        robot.isBusy = true;
        if (controllerState.Lup == ButtonStage::IDLE)
        {
            robot.stop = true;
            robot.driveDir = DriveDirection::BACKWARD;
            // if (controllerState.Rup == ButtonStage::IDLE)
            //     Retract_2Pneumatic_Pin();
            if (controllerState.Rup == ButtonStage::STEP1)
                Drop_down();
            Retract_2Pneumatic_Pin();
            Grab_Beam_up();
            // wait(200, msec);
            Spin_Robot();
            robot.stop = false;
            controllerState.Lup = ButtonStage::STEP1;
            robot.MakeY = true;
        }
        else if (controllerState.Lup == ButtonStage::STEP1)
        {
            robot.stop = true;
            wait(0.2, seconds);
            robot.stop = true;
            robot.beamState = BeamState::ON;
            if (robot.MakeY)
            {
                Ajpin();
            }
            Place_beam();
            if (!robot.MakeY)
            {
                Pin_On_the_sidelines();
            }
            controllerState.Lup = ButtonStage::IDLE;
            robot.MakeY = false;
        }
        robot.isBusy = false;
    }
}

// "when Controller ButtonLDown pressed" hat block
void LDown_pressed()
{
    Ldown_Init();
    if (robot.isBusy == false && controllerState.Lup == ButtonStage::STEP1)
    {
        Drop_down_beam();
        controllerState.Ldown = ButtonStage::IDLE;
        robot.driveDir = DriveDirection::FORWARD;
        robot.isBusy = false;
    }
}

void Drive()
{
    // if (!Controller.ButtonEUp.pressing())
    // {
    // double speedTurnAF = 0.0;
    if (robot.driveDir == DriveDirection::FORWARD)
    {
        TouchLED.setColor(red);
        // MotorRight.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) - ((Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * robot.speedB) * robot.Kspeed), percent);
        // MotorLeft.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) + ((Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * robot.speedB) * robot.Kspeed), percent);
        MotorRight.setVelocity(((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) * robot.Kspeed) - (Controller.AxisB.position() * robot.speedB), percent);
        MotorLeft.setVelocity(((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) * robot.Kspeed) + (Controller.AxisB.position() * robot.speedB), percent);
        MotorLeft.spin(forward);
        MotorRight.spin(forward);
    }
    else if (robot.driveDir == DriveDirection::BACKWARD)
    {
        TouchLED.setColor(orange);
        MotorRight.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) + (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * (robot.Kspeed - 0.05)), percent);
        MotorLeft.setVelocity((((Controller.AxisA.position() * Controller.AxisA.position()) * (Controller.AxisA.position() * 0.0001) - (Controller.AxisB.position() * Controller.AxisB.position()) * (Controller.AxisB.position() * 0.0001)) * (robot.Kspeed - 0.05)), percent);
        MotorLeft.spin(reverse);
        MotorRight.spin(reverse);
    }
    if ((Controller.AxisA.position() > -5.0 and Controller.AxisA.position() < 5) and (Controller.AxisB.position() > -5.0 and Controller.AxisB.position() < 5))
    {
        // if (speedTurnAF > 50.0 || speedTurnAF < -50.0)
        // {
        //     MotorLeft.stop(hold);
        //     MotorRight.stop(hold);
        // }
        // else
        // {
        MotorLeft.setStopping(coast);
        MotorRight.setStopping(coast);
        MotorLeft.stop();
        MotorRight.stop();
        // }
    }
    // speedTurnAF = Controller.AxisB.position();
    // }
}

int control_drive()
{
    while (true)
    {
        if (robot.stop == false)
        {
            Brain.Screen.setCursor(5, 1);
            Brain.Screen.print("startrun");
            Drive();
            if ((Controller.AxisA.position() || Controller.AxisB.position()) != 0)
            {
                if (MotorLeft.velocity(vex::velocityUnits::pct) || MotorRight.velocity(vex::velocityUnits::pct) > 5)
                {
                    Brain.Screen.setCursor(5, 1);
                    Brain.Screen.print("runing");
                }
                else
                {
                    Brain.Screen.setCursor(5, 1);
                    Brain.Screen.print("ConOKNotRun");
                }
            }
            else
            {
                Brain.Screen.setCursor(5, 1);
                Brain.Screen.print("Not runing");
            }
            // if (speed.speed == 1.0)
            // {
            //    TouchLED.setColor(black);
            // }
        }
        wait(25, msec);
    }
}

// "when started" hat block
int start()
{
    MotorPin.setPosition(0.0, degrees);
    MotorBeam.setPosition(0.0, degrees);
    MotorPin.setVelocity(80.0, percent);
    MotorPin.setMaxTorque(100.0, percent);
    MotorBeam.setVelocity(100.0, percent);
    MotorBeam.setMaxTorque(100.0, percent);
    // MotorPin.setStopping(hold);
    MotorBeam.setStopping(hold);
    MotorLeft.setStopping(coast);
    MotorRight.setStopping(coast);
    Retract_joint_pin();
    Retract_Pneumatic_Beam();
    // Pneumatic_font.pumpOn();
    Pneumatic_back.pumpOn();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Flying Bot CNX");
    MotorPin.spinFor(forward, 100, degrees);
    Drop_down();
    Drop_down_beam();
    TouchLED.setColor(red);
    Brain.Timer.reset();
    // while (true)
    // {
    //     if (!(Lup == 3.0 || stop))
    //     {
    //         Brain.Screen.setCursor(5, 1);
    //         Brain.Screen.print("startrun");
    //         Drive();
    //         if ((Controller.AxisA.position() || Controller.AxisB.position()) != 0)
    //         {
    //             if (MotorLeft.velocity(vex::velocityUnits::pct) || MotorRight.velocity(vex::velocityUnits::pct) > 5)
    //             {
    //                 Brain.Screen.setCursor(5, 1);
    //                 Brain.Screen.print("runing");
    //             }
    //             else
    //             {
    //                 Brain.Screen.setCursor(5, 1);
    //                 Brain.Screen.print("ConOKNotRun");
    //             }
    //         }
    //         else
    //         {
    //             Brain.Screen.setCursor(5, 1);
    //             Brain.Screen.print("Not runing");
    //         }
    //     }
    //     wait(25, msec);
    // }
    return 0;
}

// "when Controller ButtonFDown pressed" hat block
void FDown_pressed()
{
    Brain.Timer.reset();
    if (robot.isBusy == false)
    {
        if (robot.beamState == BeamState::OFF)
        {
            // MotorBeam.setStopping(hold);
            MotorBeam.spin(forward);
            // robot.speedB = 1.0;
            Extend_Pneumatic_Beam();
        }
        else
        {
            MotorBeam.setStopping(brake);
            MotorBeam.stop();
            // robot.speedB = 0.55;
            Retract_Pneumatic_Beam();
        }
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("Fdown_Done: %.2f", robot.nowTime);
    }
}

// "when Controller ButtonFUp pressed" hat block
void FUp_pressed()
{
    Brain.Timer.reset();
    if (robot.isBusy == false)
    {
        if (robot.pinState == PinState::OFF)
        {
            MotorPin.resetPosition();
            // Extend_2Pneumatic_Pin();
            Pneumatic_font.extend(cylinder2);
            Pneumatic_back.extend(cylinder1);
            robot.pinState = PinState::ON;
            wait(100, msec);
            MotorPin.resetPosition();
            if (robot.NewDowm == true)
                MotorPin.spinFor(forward, 42.0, degrees, false);
        }
        else if (robot.pinState == PinState::ON)
        {
            if (controllerState.Rup == ButtonStage::STEP1 && robot.Go == true)
            {
                //     MotorPin.setStopping(coast);
                //     MotorPin.stop();
                //     // wait(100, msec);
                //     Retract_2Pneumatic_Pin();
                //     MotorPin.setStopping(hold);
                //     MotorPin.stop();
                robot.Go = false;
                robot.Eup = false;
            }
            // else
            // {
            wait(100, msec);
            // Retract_2Pneumatic_Pin();
            if (robot.NewDowm == true)
            {
                MotorPin.stop(coast);
                robot.NewDowm = false;
            }
            Pneumatic_font.retract(cylinder2);
            Pneumatic_back.retract(cylinder1);
            // MotorPin.setStopping(hold);
            // MotorPin.stop();
            // }
            robot.pinState = PinState::OFF;
            wait(100, msec);
        }
        // Brain.Screen.clearLine(3, 1);
        // robot.nowTime = Brain.Timer.value();
        // Brain.Screen.setCursor(3, 1);
        // Brain.Screen.print("Fup_Done: %.2f", robot.nowTime);
    }
}

// "when Controller ButtonRUp pressed" hat block
void RUp_pressed()
{
    Rup_Init();
    if (robot.isBusy == false)
    {
        robot.isBusy = true;
        if (controllerState.Rup == ButtonStage::IDLE)
        {
            Grab_then_up();
            controllerState.Rup = ButtonStage::STEP1;
            robot.pinState = PinState::ON;
            controllerState.Rdown = ButtonStage::STEP1;
        }
        else if (controllerState.Rup == ButtonStage::STEP1)
        {
            Drop_down_Grab_Up();
            robot.pinState = PinState::ON;
            controllerState.Rup = ButtonStage::STEP1;
        }
        robot.isBusy = false;
    }
}

// "when started" hat block
int Time()
{
    Brain.Timer.reset();
    // Brain.Screen.setFont(mono20);
    while (true)
    {
        if ((Brain.Timer.value() > 300.0))
        {
            Brain.programStop();
        }
        wait(1, msec);
    }
    return 0;
}

// "when Controller ButtonRDown pressed" hat block
void RDown_pressed()
{
    Rdown_Init();
    if (robot.isBusy == false && (controllerState.Rup == ButtonStage::STEP1 || robot.Eup == false))
    {
        robot.isBusy = true;
        // if (controllerState.Rdown == ButtonStage::STEP1)
        // {
        Drop_down();
        robot.isBusy = false;
        controllerState.Rup = ButtonStage::IDLE;
        controllerState.Rdown = ButtonStage::IDLE;
        // robot.pinState = PinState::OFF;
        // }
    }
}

void EUp_pressed()
{
    // Pin_On_the_sidelines();
}

void EDown_pressde()
{
    if (robot.isBusy == false)
    {
        Pin_On_the_sidelines();
    }
}

void ControllerDChanged()
{
    if (robot.isBusy == false)
    {
        Pin_on_go();
        robot.Go = true;
    }
}

// "when Controller AxisC changed" hat block
void ControllerCChanged()
{
    // RemoteControlCodeEnabled = false;
    robot.stop = true;
    wait(100, msec);
    robot.stop = true;
    standoff();
    robot.stop = false;
    // RemoteControlCodeEnabled = true;
}
void ControllerButtonL3_pressed()
{
    if (robot.isBusy == false)
    {
        robot.Eup = false;
        Pneumatic_font.retract(cylinder1);
        Retract_joint_pin();
        MotorPin.resetPosition();
        MotorPin.setVelocity(70, percent);
        // Robot NO2 = 130.0
        // Robot NO3 = 113.0
        MotorPin.spinFor(reverse, 105.0, degrees, true);
        MotorPin.setVelocity(100, percent);
        // red_team
        Pneumatic_font.retract(cylinder2);
        // blue_team
        // Pneumatic_back.retract(cylinder1);
    }
    robot.stop = false;
    // Brain.Screen.clearLine(3, 1);
    // robot.nowTime = Brain.Timer.value();
    // Brain.Screen.setCursor(3, 1);
    // Brain.Screen.print("L3_Done: %.2f", robot.nowTime);
}

// "when Controller ButtonR3 pressed" hat block
void ControllerButtonR3_pressed()
{
    flip();
}

void Extend_Pneumatic_Beam()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_font.extend(cylinder1);
    robot.beamState = BeamState::ON;
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // // Brain.Screen.print("Beam_E: %.2f", robot.nowTime);
}

void Retract_Pneumatic_Beam()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_font.retract(cylinder1);
    robot.beamState = BeamState::OFF;
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("Beam_R: %.2f", robot.nowTime);
    return;
}
//---
void Extend_joint_pin()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_back.retract(cylinder2);
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("gPin_E: %.2f", robot.nowTime);
}

void Retract_joint_pin()
{
    // robot.nowTime = Brain.Timer.value();
    Pneumatic_back.extend(cylinder2);
    // Brain.Screen.clearLine(2, 1);
    // Brain.Screen.setCursor(2, 1);
    // Brain.Screen.print("gPin_R: %.2f", robot.nowTime);
}

void Extend_2Pneumatic_Pin()
{
    Pneumatic_font.extend(cylinder2);
    Pneumatic_back.extend(cylinder1);
    // robot.pinState = PinState::ON;
}

void Retract_2Pneumatic_Pin()
{
    Pneumatic_font.retract(cylinder2);
    Pneumatic_back.retract(cylinder1);
    // robot.pinState = PinState::OFF;
}

//--
int main()
{
    // Initializing Robot Configuration. DO NOT REMOVE!
    vexcodeInit();
    start();
    // register event handlers
    Controller.ButtonLUp.pressed(LUp_pressed);
    Controller.ButtonLDown.pressed(LDown_pressed);
    Controller.ButtonFDown.pressed(FDown_pressed);
    Controller.ButtonEDown.pressed(EDown_pressde);
    Controller.ButtonFUp.pressed(FUp_pressed);
    Controller.ButtonRUp.pressed(RUp_pressed);
    Controller.ButtonRDown.pressed(RDown_pressed);
    Controller.ButtonEUp.pressed(EDown_pressde);
    Controller.AxisD.changed(ControllerDChanged);
    Controller.AxisC.changed(ControllerCChanged);
    Controller.ButtonR3.pressed(ControllerButtonR3_pressed);
    Controller.ButtonL3.pressed(ControllerButtonL3_pressed);
    thread ws3_controller(control_drive);
    thread ws2_time(Time);
}