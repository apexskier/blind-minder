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


gearDiameter = 6.5;
gearDepth = 3;
splineCount = 22;
hexDiameter = 8; // (enclosing circle)
screwDiameter = 3;
screwHeadDiameter = 6;

translate([/*servoPosX + servoWidth / 2*/blindWidth / 2, heightToRod]) {
    difference() {
        cylinder(h=gearDepth + 1 + 14, d=hexDiameter + 4, $fn=60);

        union() {
            linear_extrude(gearDepth) {
                Star(splineCount, gearDiameter / 2, (gearDiameter-.4) / 2);
            }

            cylinder(h=40, d=screwDiameter, $fn=60);
            translate([0, 0, gearDepth + 1]) {
                cylinder(h=3, d=screwHeadDiameter, $fn=60);

                translate([0, 0, 2]) {
                    cylinder(h=20, d=hexDiameter, $fn=6);
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
