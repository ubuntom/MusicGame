#include <stdlib.h>
#include <GL/glew.h>
#include <unistd.h>
#include <controllerlist.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader/allShaders/allShaders.hpp>




SingleSelectController::SingleSelectController(GlobalInfo& g):
globalInfo(g),
sceneInfo(g),
selectEntryManager(g)
{
	//loadScene1(gl, this);
	
	unsigned int a = glGetError();
    if (a!=0)cout << "Cstart" << a << endl;
	
	GLuint b;
	
	
	globalInfo.setInputHandler(new BoundInputHandler());
	static_cast<BoundInputHandler*>(globalInfo.getInputHandler())->bindMouseHandler(std::bind(&SingleSelectController::receiveMouseInput, this, std::placeholders::_1, std::placeholders::_2));
	
	
	
	
	/*a = glGetError();
    if (a!=0)cout << "C0" << a << endl;*/
	
	
	glClearColor(0.6,0.6,0.0,1);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	
	time_t start = time(0);
	struct tm * now = localtime( & start );
	start_time=now->tm_sec;
	
	cout << start_time << endl;
	//exit(0);
	count=0;
	
	/*a = glGetError();
    if (a!=0)cout << "C1" << a << endl;*/
	
	//glUniformMatrix4fv(Global::shader->locs.projM, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	
	
	mouseActive=false;	
	
	
	globalInfo.genPixelProjectionMatrix((float)globalInfo.get_winW(), (float)globalInfo.get_winH());
	globalInfo.genFloatProjectionMatrix((float)globalInfo.get_winW()/globalInfo.get_winH(),1.f);
	/*controllerInfo.gs.projectionMatrix=glm::perspective(1.57f, 16.0f/9.0f, 0.5f, 500.0f);
	controllerInfo.gs.viewMatrix=glm::mat4(1.0f);
	controllerInfo.gs.timer = 0;*/
	

	

	

	float quad[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f,  1.0f,
	};
	
	float uv[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
	
	a = glGetError();
    if (a!=0)cout << "xxxxx" << a << endl;
	

	glGenVertexArrays(1, &vao);
	glGenBuffers(1,&v);
	glGenBuffers(1,&u);
	
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat) * 2, quad, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, u);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat) * 2, uv, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	

	
	a = glGetError();
    if (a!=0)cout << "VA02x" << a << endl;
	
	glm::mat4 m = glm::ortho(-1.f,1.f,-1.f,1.f,-1.f,1.f);
	cout << "(";
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			cout << m[i][j] << ", ";
		}
	}
	
	
	//voxelCloud.loadData("clouddata");
	
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	selectEntryManager.push(new SingleSongEntry(sceneInfo));
	
	abp = new AnimationBeatPulsar(globalInfo, GraphicID::osu_logo_small);
	
	//bzt = new BezierTest(globalInfo);
	
	
	
	
}

void SingleSelectController::update()
{
	globalInfo.beginFrame();
	unsigned int a = glGetError();
	if (a!=0)cout << "Exx" << a << endl;
	
	double framestart=glfwGetTime();
	globalInfo.setFrameStart(framestart);
	
	
	
	glDisable(GL_DEPTH_TEST);
	

	glViewport( 0, 0, globalInfo.getWindowWidth(), globalInfo.getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2,0.5,1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	
	a = glGetError();
	if (a!=0)cout << "CLEAR " << a << endl;
	
	
	globalInfo.stepPhasor();

	globalInfo.shaderManager->bind(0);
	
	
	

	
	
	selectEntryManager.render();
	
	//abp->render();
	
	//bzt->render();
	
	a = glGetError();
	if (a!=0)cout << "CHRender1 " << a << endl;
	


	globalInfo.endFrame();
	
	glfwSwapBuffers(globalInfo.getWindow());
	
	a = glGetError();
	if (a!=0)cout << "CHSwap " << a << endl;

	float interval = glfwGetTime() - framestart;
	double delay = 1/interval;
	
	if(delay<min)min=delay;
	if(delay>fmax)fmax=delay;
	
	rolling+=delay;
	frames+=1;
	
	//globalInfo.genFloatProjectionMatrix(1368./768.*frames,1.f*frames);
	
	float avg = rolling/frames;
	
	//cout << min << " " << max << " " << avg << endl;
	
	double continuetime = framestart+1.0/120.0;

	
	while (glfwGetTime() < continuetime){
		usleep(500);
		glfwPollEvents();
	}
	
	//cout << "Ts " << glfwGetTime() << endl;

	//usleep(1000000);
	
	//cout << "Tt " << glfwGetTime() << endl;

	
	
	//exit(0);
	
	//cout << 1/(glfwGetTime() - framestart);
}


void SingleSelectController::receiveMouseInput(int b, int a)
{
	selectEntryManager.handleClick(b, a);
}
