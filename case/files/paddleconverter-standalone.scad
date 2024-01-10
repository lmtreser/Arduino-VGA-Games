// This file was processed by resolve-include.py [https://github.com/arpruss/miscellaneous-scad/blob/master/scripts/resolve-include.py] 
// to include  all the dependencies inside one file.

topButtons = false;
includeBottom = true;
includeLid = true;
addToHeight = 15;
width = 55;
length = 56;
corner = 3;
wall = 1.25;
cableCutout = 6;
usbHoleWidth = 11.3;
usbHoleHeight = 5.65;
usbHoleCenterOffsetFromBase = 6.1;
screwHole = 3;
screwBiggerHole = 4;
screwHeadDiameter = 8;
screwHeadInset = 2;
screwBearingWallMinimum = 3;
screwHoleWall = 2;
screwGrabLength = 6;
pcbScrewHorizontalSpacing = 30.7; // pcbholdernarrower
pcbScrew1DistanceFromFront = 5;
pcbScrew2DistanceFromFront = 50;
pcbScrewHole = 4;
pcbScrewHeadDiameter = 7;
pcbScrewHeadInset = 2;
lidTolerance = 0.2;
buttonWellWidth = 6.5;
buttonWellLength = 6.5;
buttonWellHeight = 2.75;
buttonLegHoleDiameter = 1.75;
buttonLegXSpacing = 4.6;
buttonLegYSpacing = 6.21;
buttonSpacing = 16;
buttonColumns = 2;
buttonRows = 2;

module end_of_parameters_dummy() {}


//BEGIN DEPENDENCY: use <roundedSquare.scad>;
module roundedSquare(size=[10,10], radius=1, center=false, $fn=16) {
    size1 = (size+0==size) ? [size,size] : size;
    if (radius <= 0) {
        square(size1, center=center);
    }
    else {
        translate(center ? -size1/2 : [0,0])
        hull() {
            translate([radius,radius]) circle(r=radius);
            translate([size1[0]-radius,radius]) circle(r=radius);
            translate([size1[0]-radius,size1[1]-radius]) circle(r=radius);
            translate([radius,size1[1]-radius]) circle(r=radius);
        }
    }
}

module roundedCube(size=[10,10,10], radius=1, center=false, $fn=16) {
    linear_extrude(height=size[2])
    roundedSquare(size=[size[0],size[1]], radius=radius, center=center);
}

module roundedOpenTopBox(size=[10,10,10], radius=2, wall=1, solid=false) {
    render(convexity=2)
    difference() {
        linear_extrude(height=size[2]) roundedSquare(size=[size[0],size[1]], radius=radius);
        if (!solid) {
            translate([0,0,wall])
            linear_extrude(height=size[2]-wall)
            translate([wall,wall]) roundedSquare(size=[size[0]-2*wall,size[1]-2*wall], radius=radius-wall);
        }
    }
}

//END DEPENDENCY: use <roundedSquare.scad>;


//BEGIN DEPENDENCY: use <pointHull.scad>;
function _slice(list,start,end=undef) =
    let(end = end==undef?len(list):end)
    [for(i=[start:1:end-1]) list[i]];

function _delete(list,pos) =
    let(l=len(list))
    pos == 0 ? [for(i=[1:1:l-1]) list[i]] :
    pos >= l-1 ? [for(i=[0:1:pos-1]) list[i]] :
    concat([for(i=[0:1:pos-1]) list[i]],
        [for(i=[pos+1:1:l-1]) list[i]]);

function _areCollinear(a,b,c) = cross(b-a,c-a) == [0,0,0];

function _areCoplanar(a,b,c,d) = (d-a)*cross(b-a,c-a) == 0;

function _findNoncollinear(list,point1,point2,pos=0) =
    pos >= len(list) ? undef :
    ! _areCollinear(point1,point2,list[pos]) ? pos :
    _findNoncollinear(list,point1,point2,pos=pos+1);

function _findNoncoplanar(list,point1,point2,point3,pos=0) =
   pos >= len(list) ? undef :
   ! _areCoplanar(point1,point2,point3,list[pos]) ? pos :
        _findNoncoplanar(list,point1,point2,point3,pos=pos+1);

function _isBoundedBy(a,face,strict=false) =
    cross(face[1]-face[0],face[2]-face[0])*(a-face[0]);

function _makeTet(a,b,c,d) =
    _isBoundedBy(d,[a,b,c]) >= 0 ?
        [[a,b,c],[b,a,d],[c,b,d],[a,c,d]] :
        [[c,b,a],[d,a,b],[d,b,c],[d,c,a]];

function _findTri(list) =
    let(l2=_unique(list))
    assert(len(l2)>=3)
    let(a=l2[0],
        b=l2[1],
        l3=_slice(l2,2),
        ci=_findNoncollinear(l3,a,b),
        c=assert(ci != undef) l3[ci],
        l4=_delete(l3,ci))
        [[a,b,c],l4];

function _findTet(list) =
    let(ft=_findTri(list),
        tri=ft[0],
        l4=ft[1],
        di=assert(len(l4)>0) _findNoncoplanar(l4,tri[0],tri[1],tri[2]),
        d=assert(di != undef) l4[di],
        l5=_delete(l4,di))
        [_makeTet(tri[0],tri[1],tri[2],d),l5];

function _find(list,value) =
    let(m=search([value],list,1)[0])
    m==[] ? undef : m;

function _unique(list,soFar=[],pos=0) =
    pos >= len(list) ? soFar :
    _find(soFar,list[pos]) == undef ? _unique(list,soFar=concat(soFar,[list[pos]]),pos=pos+1) :
    _unique(list,soFar=soFar,pos=pos+1);

function _makePointsAndFaces(triangles) =
    let(points=_unique([for(t=triangles) for(v=t) v]))
        [points, [for(t=triangles) [for(v=t) _find(points,v)]]];

function _sameSide(p1,p2, a,b) =
    let(cp1 = cross(b-a, p1-a),
        cp2 = cross(b-a, p2-a))
        cp1*cp2 >= 0;

function _insideTriangle(p, t) =
    _sameSide(p,t[0],t[1],t[2]) &&
    _sameSide(p,t[1],t[0],t[2]) &&
    _sameSide(p,t[2],t[0],t[1]);


function _satisfiesConstraint(p, triangle) =
    let(c=_isBoundedBy(p, triangle))
    c > 0 || (c == 0 && _insideTriangle(p, triangle));


function _insidePoly(p, triangles, pos=0) =
    pos >= len(triangles) ? true :
    !_satisfiesConstraint(p, triangles[pos]) ? false :
    _insidePoly(p, triangles, pos=pos+1);

function _outerEdges(triangles) =
        let(edges=[for(t=triangles) for(e=[[t[0],t[1]],
                [t[1],t[2]],
                [t[2],t[0]]]) e])
        [for(e=edges) if(undef == _find(edges,[e[1],e[0]])) e];

function _unlit(triangles, p) = [for(t=triangles) if(_isBoundedBy(p, t) >= 0) t];

function _addToHull(h, p) =
    let(unlit = _unlit(h,p),
        edges = _outerEdges(unlit))
        concat(unlit, [for(e=edges) [e[1],e[0],p]]);

function _expandHull(h, points, pos=0) =
    pos >= len(points) ? h :
    !_insidePoly(points[pos],h) ?  _expandHull(_addToHull(h,points[pos]),points,pos=pos+1) :
    _expandHull(h, points, pos=pos+1);

function pointHull3D(points) =
    let(ft=_findTet(points))
        _expandHull(ft[0], ft[1]);

function _traceEdges(edgeStarts,edgeEnds,soFar=undef) =
    soFar==undef ? _traceEdges(edgeStarts,edgeEnds,[edgeStarts[0]]) :
    assert(soFar[len(soFar)-1] != undef)
    len(soFar)>1 && soFar[len(soFar)-1] == soFar[0] ? _slice(soFar,1) :
    _traceEdges(edgeStarts,edgeEnds,soFar=concat(soFar,[edgeEnds[_find(edgeStarts,soFar[len(soFar)-1])]]));

function _project(v) = [v[0],v[1]];

function pointHull2D(points) =
    let(
        p3d = concat([[0,0,1]],
            [for(p=points) [p[0],p[1],0]]),
        h = pointHull3D(p3d),
        edgeStarts = [for(t=h) if(t[0][2]==1 || t[1][2]==1 || t[2][2]==1)
                t[0][2]==1 ? _project(t[1]) :
                t[1][2]==1 ? _project(t[2]) :
                _project(t[0]) ],
        edgeEnds = [for(t=h) if(t[0][2]==1 || t[1][2]==1 || t[2][2]==1)
                t[0][2]==1 ? _project(t[2]) :
                t[1][2]==1 ? _project(t[0]) :
                _project(t[1]) ])
        _traceEdges(edgeStarts,edgeEnds,soFar=[edgeStarts[0]]);

function pointHull(points) =
    len(points[0]) == 2 ? pointHull2D(points) :
        pointHull3D(points);

module pointHull(points) {
    if (len(points[0])==2) {
        polygon(pointHull2D(points));
    }
    else {
        paf = _makePointsAndFaces(pointHull3D(points));
        polyhedron(points=paf[0],faces=paf[1]);
    }
}

function extractPointsFromHull(h) =
    _unique(
        is_num(h[0][0]) ? h : [for(t=h) for(v=t) v] );

function _d22(a00,a01,a10,a11) = a00*a11-a01*a10;

function _determinant3x3(m) = m[0][0]*_d22(m[1][1],m[1][2],m[2][1],m[2][2])
    -m[0][1]*_d22(m[1][0],m[1][2],m[2][0],m[2][2])
    +m[0][2]*_d22(m[1][0],m[1][1],m[2][0],m[2][1]);

function _determinant2x2(m) = _d22(m[0][0],m[0][1],m[1][0],m[1][1]);

// Cramer's rule
// n.b. Determinant of matrix is the same as of its transpose
function _solve3(a,b,c) =
    let(det=_determinant3x3([a[0],b[0],c[0]]))
    det == 0 ? undef :
    let(rhs=[a[1],b[1],c[1]],
        col0=[a[0][0],b[0][0],c[0][0]],
        col1=[a[0][1],b[0][1],c[0][1]],
        col2=[a[0][2],b[0][2],c[0][2]])
    [_determinant3x3([rhs,col1,col2]),
    _determinant3x3([col0,rhs,col2]),
    _determinant3x3([col0,col1,rhs])]/det;

function _solve2(a,b) =
    let(det=_determinant2x2([a[0],b[0]]))
    det == 0 ? undef :
    let(rhs=[a[1],b[1]],
        col0=[a[0][0],b[0][0]],
        col1=[a[0][1],b[0][1]])
    [_determinant2x2([rhs,col1]),
    _determinant2x2([col0,rhs])]/det;

function _satisfies(p,constraint) =
    p*constraint[0] <= constraint[1];

function _linearConstraintExtrema(constraints,constraintsMarked=false) =
        let(c=_unique(constraints),
            n=len(c))
        len(c[0][0]) == 3 ?
        [
        for(i=[0:1:n-1]) for(j=[i+1:1:n-1]) for(k=[j+1:1:n-1]) let(p=_solve3(c[i],c[j],c[k])) if(p!=undef && _satisfiesAll(p,c,except1=i,except2=j,except3=k)) constraintsMarked?[p,[i,j,k]]:p
        ] :
        [
        for(i=[0:1:n-1]) for(j=[i+1:1:n-1]) let(p=_solve2(c[i],c[j])) if(p!=undef && _satisfiesAll(p,c,except1=i,except2=j)) constraintsMarked?[p,[i,j,-1]]:p
        ];

function _satisfiesAll(p,c,except1=undef,except2=undef,except3=undef,pos=0) =
        pos >= len(c) ? true :
        except1 == pos || except2 == pos || except3 == pos || _satisfies(p,c[pos]) ? _satisfiesAll(p,c,except1,except2,except3,pos=pos+1) :
        false;


module linearConstraintShape(constraints) {
    pointHull(_linearConstraintExtrema(constraints));
}

// 3D inly
function _onPlane(v,planeIndex,data,pos=0) =
    pos >= len(data) ? false :
    data[pos][0] == v && (data[pos][1][0] == planeIndex || data[pos][1][1] == planeIndex || data[pos][1][2] == planeIndex) ? true :
    _onPlane(v,planeIndex,data,pos=pos+1);

// 3D only
function _getFaceOnPlane(planeIndex,triangles,data) =
    let(trianglesOnPlane = [for(t=triangles) if (_onPlane(t[0],planeIndex,data) &&
        _onPlane(t[1],planeIndex,data) &&
        _onPlane(t[2],planeIndex,data)) t],
        outer = _outerEdges(trianglesOnPlane),
        edgeStarts = [for(e=outer) e[0]],
        edgeEnds = [for(e=outer) e[1]]
    ) len(outer) == 0 ? undef : echo("se",edgeStarts,edgeEnds) _traceEdges(edgeStarts,edgeEnds);

// 3D only, does not work if there are multiple planes defined by the same constraint
function linearConstraintPointsAndFaces(constraints) =
    let(data=_linearConstraintExtrema(constraints,constraintsMarked=true),
    extremePoints=[for(d=data) d[0]],

    triangles=pointHull(extremePoints),
    hullPoints=extractPointsFromHull(triangles),
    faces=[for (i=[0:len(constraints)-1]) [for(v=_getFaceOnPlane(i,triangles,data)) if(v != undef) _find(hullPoints,v)]]
    )
    [hullPoints,faces];

function hullPoints(points) =
    extractPointsFromHull(pointHull(points));

module dualHull(points) {
    p = hullPoints(points);
    constraints = [for(v=p) [v,v*v]];
    linearConstraintShape(constraints);
}


//END DEPENDENCY: use <pointHull.scad>;



module dummy() {}

nudge = 0.001;
$fn = 32;

pcbScrews = [ [pcbScrewHorizontalSpacing/2, pcbScrew1DistanceFromFront ],
    [-pcbScrewHorizontalSpacing/2, pcbScrew1DistanceFromFront ],
    [pcbScrewHorizontalSpacing/2, pcbScrew2DistanceFromFront ],
    [-pcbScrewHorizontalSpacing/2, pcbScrew2DistanceFromFront ]
];

lidPillar=screwHole+2*screwHoleWall;

lidScrews = [[-width/2+lidPillar/2,lidPillar/2],[width/2-lidPillar/2,lidPillar/2],[-width/2+lidPillar/2,length-lidPillar/2],[width/2-lidPillar/2,length-lidPillar/2]];
lidScrewAngles = [45,135,-45,-135];

extraHeight = pcbScrewHeadInset+max(wall,screwBearingWallMinimum)-wall+wall;

bottomHeight = addToHeight+extraHeight;

module base(wall) {
    roundedSquare([width+2*wall,length+2*wall],radius=lidPillar/2,center=true);
}

module buttonBezel(positive=true) {
    if (positive)  {
        h = buttonWellHeight+nudge;
    translate([0,0,wall-nudge]) {
        difference() {
            pointHull(
                [for (w=[0,h]) for (i=[-1,1]) for(j=[-1,1]) [i*(buttonWellWidth/2+w),j*(buttonWellLength/2+w),h-w]]);
            cube([buttonWellWidth,buttonWellLength,h*3],center=true);
            }
        }
    }
    else {
        translate([0,0,-nudge]) {
            h = buttonWellHeight+wall+2*nudge;
            for (i=[-1,1]) for(j=[-1,1]) translate([i*buttonLegXSpacing/2,j*buttonLegYSpacing/2,0]) cylinder(d=buttonLegHoleDiameter,h=h);
        }
    }
}

module doTopButtons(positive=true) {
    if (topButtons) {
        for (i=[0:buttonColumns-1]) for (j=[0:buttonRows-1]) translate([-(buttonColumns-1)*buttonSpacing/2+buttonSpacing*i,length/2-(buttonRows-1)*buttonSpacing/2+buttonSpacing*j,0]) buttonBezel(positive);
    }
}

module lid() {
    difference() {
        union() {
            translate([0,length/2,0])
            linear_extrude(height=wall) base(-lidTolerance);
            if (!topButtons) linear_extrude(height=screwBearingWallMinimum) intersection() {
                for(s=lidScrews) translate(s) circle(d=screwHole+2*screwHoleWall);
                translate([0,length/2,0]) base(-lidTolerance);
            }
            doTopButtons(positive=true);
        }
        for(s=lidScrews) translate(s) translate([0,0,-nudge]) cylinder(d=screwBiggerHole,h=(topButtons ? wall : max(screwBearingWallMinimum,wall))+2*nudge);
        doTopButtons(positive=false);
    }
}

module box(height) {
    translate([0,length/2,0]) {
        linear_extrude(height=wall) base(wall);
        linear_extrude(height=height+wall)
        difference() {
            base(wall);
            base(0);
        }
    }
}

module screwHead(positive,headDiameter,headInset,hole) {
    h = headInset+max(wall,screwBearingWallMinimum);
    if (positive) {
        cylinder(d=headDiameter+2*screwHoleWall,h=h);
    }
    else {
        translate([0,0,-nudge]) {
            cylinder(d=headDiameter,h=headInset);
            cylinder(d=hole,h=h+2*nudge);
        }
    }
}

module bottom() {
    difference() {
        union() {
            box(bottomHeight);
            for (p=pcbScrews)
                translate(p) screwHead(true,pcbScrewHeadDiameter,pcbScrewHeadInset,pcbScrewHole);
        }
        for (p=pcbScrews)
            translate(p) screwHead(false,pcbScrewHeadDiameter,pcbScrewHeadInset,pcbScrewHole);
    }
    for(i=[0:1:len(lidScrews)-1])
        translate(lidScrews[i]) rotate([0,0,lidScrewAngles[i]]) screwHolder();
}

module screwHolder() {
    d = lidPillar;
    h = screwGrabLength+d;
    translate([0,0,bottomHeight-(topButtons ? wall : max(wall,screwBearingWallMinimum ))-h+wall])
    difference() {
        intersection() {
            cylinder(d=screwHole+2*screwHoleWall+nudge,h=h);
            pointHull([[-d/2,-d/2,0],[-d/2,d/2,0],[d,-d/2,d],[d,d/2,d],[-d/2,-d/2,h],[-d/2,d/2,h],[d/2,d/2,h],[d/2,-d/2,h]]);
        }
        translate([0,0,-h]) cylinder(d=screwHole,h=3*h);
    }
}

module usbCutout() {
    h0 = pcbScrewHeadInset+max(wall,screwBearingWallMinimum);
    translate([0,0,h0+usbHoleCenterOffsetFromBase]) cube([usbHoleWidth,usbHoleHeight,wall*3],center=true);
}


if (includeBottom)
difference() {
    bottom();
    usbCutout();
    translate([0,length,bottomHeight-cableCutout/2])
    rotate([270,0,0])
    translate([0,0,-5])
    rotate([0,0,180])
    {
    cylinder(d=cableCutout,h=100);
    translate([-cableCutout/2,0,0]) cube([cableCutout,cableCutout+100,100]);
    }
}

if (includeLid) translate([width+wall+5,0,0]) lid();

