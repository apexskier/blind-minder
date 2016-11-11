servoHeight = 11.8;
servoWidth = 23.2;
pegGap = 1.6;
pegDiameter = 3.5;
pegHeight = 4;

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

// linear_extrude(partThickness) {
//     difference() {
//         square([blindWidth, heightToRod + servoHeight / 2]);
//
//         translate([servoPosX, heightToRod - servoHeight / 2 - .5]) {
//             square([servoWidth, servoHeight + .5]);
//         }
//         translate([blindWidth / 2, 0]) {
//             resize([blindWidth - 8, heightToRod]) {
//                 circle($fn=60);
//             }
//         }
//     }
// }
//
// linear_extrude(partThickness + pegHeight) {
//     translate([servoPosX, 0]) {
//         translate([0, heightToRod]) {
//             translate([-pegDiameter/2 - pegGap, 0]) {
//                 circle(d=pegDiameter, $fn=60);
//             }
//             translate([servoWidth + pegDiameter/2 + pegGap, 0]) {
//                 circle(d=pegDiameter, $fn=60);
//             }
//         }
//     }
// }


gearDiameter = 5.5;
gearDepth = 3;
splineCount = 22;
hexDiameter = 7.5; // (enclosing circle)
screwDiameter = 3;
screwHeadDiameter = 6;

translate([/*servoPosX + servoWidth / 2*/blindWidth / 2, heightToRod]) {
    difference() {
        cylinder(h=gearDepth + 1 + 14, d=hexDiameter + 8, $fn=60);

        // linear_extrude(gearDepth) {
        //     Star(splineCount, gearDiameter / 2, (gearDiameter-.4) / 2);
        // }

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
