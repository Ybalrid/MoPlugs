Particle System

 This a new version of particle shader in MoBu based on gpu opengl
 transform feedback extension (OpenGL 3.0+)

 The main features of this shader are
 + 1 - performance (fully loaded on gpu, no transferring back array of data)
 + 2 - simple collisions
 + 3 - forces (gravity, wind, vortex, etc.)
 + 4 - geometry instancing, not only simple facing
 5 - support of particle rotations
 + 6 - support of texturing particles
 7 - support of particle spawn on event

 Make a test like NVidia turbulence sample, something simular

 

 TIP
 1) for transfer real 4 32bit float values, you should specify GL_RGBA32F internal format!

 TODO
 - add texture matrix support for the emitter texture

 - shadows slices
 - depth sorting for correct transparency output
 - self collisions
 - rotation and rotation velocity
 - sprite sheets
 - vector fields, good article in the sub-folder
 - soft particles, mix with a scene depth buffer
 - events, triggers, story commands !

 + check wind force with lots of particles in frame (in shader particle Id is divided by 3000.0f)


 13.12.2013
    ������ ���������� ��������, ��� ��������� �������� ��� ��������� ����������. ��� �� �����, ���� ��-��� �����.
  ��������� ���� �� ������� ����� ���� ������ ���������, ��� �� ������������� ������ � ������ ������.
    �� ������ ������ ������������� ����� �� ������� ��������������, ����. ���� ����� ����������� ��� ������� ��������
������, �������� � ������� 3� �������� ����. ������ �������� ����� ���� ���������, ����� ������ ��������� ����� ��������
��� velocity ������ � ����.
	������ ������ ������ � ���������� ������. �� ������ ������ ������������� ��� �����������, � �������� �������� ������� ������.
����� ����������� �������� �������� �����-����� 3 ��� 4� ������� ��� �� ������ �� ������ ������ ����������� ��� ��������, �����
������ ��� ���������� � ���������� ����� � ����� ��������� ����� ������������� � ��� ����� ����� ����������� �� �������.
	����� ����������� ����������� ������ ��������� ���, ����� �������� ������ ��� ��������� � ������������ � ������ ��� ��������,
�������� ��� ������ ���� ��� ��� �������� (������� ������ �� ������, ��� ��������� ����������� ������ �������)
 
  + add support of transparency modes (old task - sort particles and draw after everything)

LOG

30.08.2016
 + added a first test of self collision shader
 * fix a bug with displayable launchers
 * fix a bug with context changes for terrain

26.08.2016
 - bindless technique not connected
 * fix terrain collision depth rendering and colliding
 * connected terrain collision

23.08.2016
 * added preGenerated (light) version of emit (particle uses reset frame launcher generated data)
 * connected forces and collisions (only sphere)
 * connected size attenuation

22.08.2016
 * more tweaks on stability and functionality, debugging

20.08.2016
 * enhancement on particle logic with shader-models binding

19.08.2016
 + added UI layout for the gpu shader
 * make auto-column arranging for UI elements

13.08.2016
 * updated behaviour for start direction, velocity and their randomness

12.08.2016
 * removed fixedPosition property, matrix multiply should be like that gTM * vec4(pos.xyz, 1.0)

09.08.2016
 + attach compute shader for the particles simulation

06.08.2016
 * some logic fixes for Particle Rate and Generated Reset Count
 + emit from surface, enherit surface diffuse pixels color

28.07.2016
 * devide evaluate shader into two steps: emit and simulate

24.07.2016
 * terrain works well with some bug fixes (selection in the viewport) and glslfx shader (with bindless texture)
 * fixed selection issue for all other custom models

23.07.2016
 * WIP: rebuilding shaders into GLSLFX and some code refactoring

23.02.2014
* some bug fix with collisions

+ added stretched billboard, thanks to The LIttle Grasshopper website and the article about stretched billboards
http://prideout.net/blog/?p=61#billboards

11.02.2014
 * working on the stretch by particle motion

27.11.2014
 * fix gen particle position during movement
 * fix wind force, add noiseFreq and noiseSpeed properties

22.07.2014
 + include project into the moplugs solution

10.03.2014
 * update algorithm for wind force (taken from TressFX)

04.01.2014
 + move user objects into the "Browsing/Templates/Physical Properties/Particles"

21.12.2013
 * instancing fixes
 * fix blinking particles at the end
 * correct billboard shape generation
 * fix reset button
 * add property "Fix Emitter Position" to fix internal shader bug when emitting from vertices
 * fix playing on low framerates
 * add float precision for terrain collision
 * fix low performance in scene with lots of elements

18.12.2013
 + add terrain collision support
 + add terrain collision object and mode for automatic heightfield generation
 * add color and size dynamic properties

13.12.2013
 + add noise to wind force
 + add rate parameter, time step and functionality to play mode

12.12.2013
 + add simple sphere collision
 + add simple wind force

09.12.2013
 + add a lot of parameters (mass, damping, floor collision, etc)
 + add normal buffer and get start direction from mesh normals
 + add normalized life time parameter for render shaders

07.12.2013
 * use emitter position (vertex emitter mode)
 + transparency mode support

26.11.2013
 add instancing
 billboard with textures
 support deform meshes as emitters
 calculation on gpu

24.11.2013
 add some properties for control particle simulation and visualization

19.11.2013
 first iteration of gpu based particle system