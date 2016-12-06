use <publicDomainGearV1.1.scad>;

servoHeight = 11.8;
servoWidth = 23.2;

pegGap = 1.6;
pegDiameter = 3.5;
pegHeight = 4;

module output_mount() {
    largeSpokeOffset = 16.2;
    //
    //   largeSpokeOffset
    //          |
    //          v
    // |    .[----o--].       |  ]- servoHeight \
    // |    ^     ^   ^       |                 |- heightToRod
    // |    |     |   |       |                 |
    // |   peg    |  peg      |                 /
    //          center
    //
    //  <---- blindWidth ---->

    // 45.5
    blindWidth = 55;
    heightToRod = 21;

    partThickness = 10;

    servoPosX = (blindWidth / 2) - largeSpokeOffset;

    linear_extrude(partThickness) {
        difference() {
            square([blindWidth, heightToRod + servoHeight / 2]);

            translate([servoPosX, heightToRod - servoHeight / 2 - .5]) {
                square([servoWidth, servoHeight + .5]);
            }
            translate([blindWidth / 2, 0]) {
                resize([blindWidth - 8, heightToRod]) {
                    circle($fn=60);
                }
            }
        }
    }

    linear_extrude(partThickness + pegHeight) {
        translate([servoPosX, 0]) {
            translate([0, heightToRod]) {
                translate([-pegDiameter/2 - pegGap, 0]) {
                    circle(d=pegDiameter, $fn=60);
                }
                translate([servoWidth + pegDiameter/2 + pegGap, 0]) {
                    circle(d=pegDiameter, $fn=60);
                }
            }
        }
    }
}

module input_mount() {
    largeSpokeOffset = 16.2;
    //
    //   largeSpokeOffset
    //          |
    //          v
    // |    .[----o--].       |  ]- servoHeight \
    // |    ^     ^   ^       |                 |- heightToRod
    // |    |     |   |       |                 |
    // |   peg    |  peg      |                 /
    //          center
    //
    //  <---- blindWidth ---->

    // 45.5
    blindWidth = 55;
    posY = 5.8;

    partThickness = 2;
    partCutoutThickness = 10.6;
    wingWidth = 5.3;
    wingThickness = 2.8;

    servoPosX = wingWidth + 0;

    spurLength = 15;
    spurHeight = 4.6;
    spurThickness = 2;
    spurExtraDiameter = 1;
    spurX = 3.8;
    translate([-(spurLength + spurExtraDiameter), spurX, 0]) {
        translate([spurExtraDiameter / 2, spurHeight, 0])
            cylinder(d=spurExtraDiameter, h=spurThickness, $fn=60);
        difference() {
            cube([spurLength + spurExtraDiameter, spurHeight, spurThickness]);

            translate([spurLength / 2, 0, 0])
                scale([-1, -1, 1])
                    rotate(-10, [0, 0, 1])
                        cube([spurLength + spurExtraDiameter + 10, spurHeight, spurThickness]);
        }
    }

    difference() {
        union() {
            linear_extrude(partThickness + partCutoutThickness) {
                difference() {
                    square([servoWidth + servoPosX + 5, posY + servoHeight]);

                    translate([servoPosX, posY]) {
                        square([servoWidth, servoHeight]);
                    }
                    square([2, spurX]);
                }
            }
        }
        translate([0, 0, partCutoutThickness - wingThickness]) {
            linear_extrude(wingThickness + 10) {
                translate([.4, posY]) {
                    square([servoWidth + servoPosX - .4 + wingWidth, servoHeight]);
                }
            }
        }
    }
}

input_mount();

screwDiameter = 3;
screwHeadDiameter = 6;

module control_pully() {
    pullyDiameter = 16;
    additionalRadius = 3.75;
    pullyWidth = 15.7;
    baseThickness = 2.5;

    sqCutoutWidth = 5.3;
    sqNotch = 2.4;
    sqNotchDepth = 1.8;

    difference() {
        union() {
            cylinder(h=pullyWidth, d=pullyDiameter, $fn=60);
            cylinder(h=baseThickness, d=pullyDiameter + additionalRadius * 2, $fn=60);
            translate([0, 0, pullyWidth - baseThickness / 2]) {
                gear(
                    mm_per_tooth = 3,
                    number_of_teeth = 22,   //total number of teeth around the entire perimeter
                    thickness = baseThickness,   //thickness of gear in mm
                    backlash = 0.2
                );
            }
        }

        translate([0, 0, pullyWidth / 2]) {
            rotate(90, [1, 0, 0]) {
                cylinder(h=100, d=5, $fn=60);
            }
        }

        translate([0, 0, -1]) {
            linear_extrude(height=100) {
                square(sqCutoutWidth, center=true);
                translate([0, sqCutoutWidth / 2 - sqNotch / 2 + sqNotchDepth, 0]) {
                    square(sqNotch, center=true);
                }
            }
        }
    }
}

module servo_gear_cutout() {
    gearDiameter = 6.8;
    splineCount = 22;

    linear_extrude(100) {
        Star(splineCount, gearDiameter / 2, (gearDiameter-.2) / 2);
    }
}

module input_servo_attachment() {
    gearThickness = 4;

    difference() {
        union() {
            // translate([0, 0, -3]) {
            //     cylinder(h=gearThickness, d=18, $fn=60);
            // }
            translate([0, 0, gearThickness / 2]) {
                gear(
                    mm_per_tooth = 3,
                    number_of_teeth = 16,   //total number of teeth around the entire perimeter
                    thickness = gearThickness,   //thickness of gear in mm
                    backlash = 0.2
                );
            }
        }

        translate([0, 0, -1]) {
            cylinder(h=40, d=screwDiameter, $fn=60);
        }
        translate([0, 0, 1.8]) {
            servo_gear_cutout();
        }
    }
}

module output_servo_attachment() {
    hexDiameter = 7.7; // (enclosing circle)
    gearDepth = 3;

    difference() {
        cylinder(h=gearDepth + 1 + 14, d=hexDiameter + 8, $fn=60);

        // servo_gear_cutout();

        cylinder(h=40, d=screwDiameter, $fn=60);
        translate([0, 0, gearDepth + 1]) {
            cylinder(h=3, d=screwHeadDiameter, $fn=60);

            translate([0, 0, 2]) {
                cylinder(h=20, d=hexDiameter, $fn=6);
            }
        }
        translate([0, 0, 0]) {
            cylinder(h=7, d=screwHeadDiameter, $fn=60);

            translate([0, 0, 4]) {
                cylinder(h=20, d=hexDiameter, $fn=6);
            }
        }

        servoCutout();
    }
}

module servoCutout() {
    translate([0, 0, -1])
    linear_extrude(2 + 1) {
        offset(0.2, $fn=40) {
            difference() {
                paddleWidth = 4;
                centerSquareOverlap = 6;
                centerSquareWidth = paddleWidth + centerSquareOverlap * 2;

                union() {
                    square([paddleWidth, 20], center=true);
                    square([20, paddleWidth], center=true);

                    square(centerSquareWidth, center=true);
                }

                for (deg=[0:90:270]) {
                    rotate(deg + 45, [0, 0, 1]) {
                        translate([sqrt(2 * pow((centerSquareWidth - centerSquareOverlap) / 2, 2)), 0]) {
                            circle(d=centerSquareOverlap, $fn=60);
                            rotate(45, [0, 0, 1]) {
                                translate([-centerSquareOverlap/2, -centerSquareOverlap]) {
                                    square(centerSquareOverlap);
                                }
                                translate([0, -centerSquareOverlap / 2]) {
                                    square(centerSquareOverlap);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// points = number of points (minimum 3)
// outer  = radius to outer points
// inner  = radius to inner points
module Star(points, outer, inner) {
    // polar to cartesian: radius/angle to x/y
    function x(r, a) = r * cos(a);
    function y(r, a) = r * sin(a);

    // angular width of each pie slice of the star
    increment = 360/points;

    union() {
        for (p = [0 : points-1]) {
            // outer is outer point p
            // inner is inner point following p
            // next is next outer point following p
            x_outer = x(outer, increment * p);
            y_outer = y(outer, increment * p);
            x_inner = x(inner, (increment * p) + (increment/2));
            y_inner = y(inner, (increment * p) + (increment/2));
            x_next  = x(outer, increment * (p+1));
            y_next  = y(outer, increment * (p+1));
            polygon(
                points=[
                    [x_outer, y_outer],
                    [x_inner, y_inner],
                    [x_next, y_next],
                    [0, 0]
                ],
                paths  = [[0, 1, 2, 3]]
            );
        }
    }
}
