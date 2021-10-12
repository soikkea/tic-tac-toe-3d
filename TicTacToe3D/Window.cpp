#include "Window.h"
#include "raylib.h"
#include <raymath.h>
#include <algorithm>
#include <sstream>

void Window::SelectCube(const Vec3& pos)
{
	if (!(
		pos.x >= 0 && pos.x <= 2 &&
		pos.y >= 0 && pos.y <= 2 &&
		pos.z >= 0 && pos.z <= 2))
	{
		return;
	}

	auto selectedCubeName = "cube_" + getCubeIdString(selectedCube_);
	models_[selectedCubeName].mesh.setColor(BLANK);

	selectedCube_ = pos;

	selectedCubeName = "cube_" + getCubeIdString(selectedCube_);
	models_[selectedCubeName].mesh.setColor({ 135,60,190, 100 });
}

std::string Window::getCubeIdString(const Vec3& pos)
{
	std::ostringstream ss;
	ss << pos.x << pos.y << pos.z;
	return ss.str();
}

bool Window::useCube(const Vec3& pos)
{
	if (!(
		pos.x >= 0 && pos.x <= 2 &&
		pos.y >= 0 && pos.y <= 2 &&
		pos.z >= 0 && pos.z <= 2))
	{
		return false;
	}
	std::string modelName = "flat_" + getCubeIdString(pos);
	if (selectedCubes_.count(modelName) > 0)
	{
		return false;
	}
	selectedCubes_.insert(modelName);

	std::string playerSymbol{ game_.GetCurrentPlayer().GetSymbol() };

	models_[modelName] = util::Model{ meshes_[playerSymbol], Vector3Add({-1, -1, -1}, {(float)pos.x, (float)pos.y, (float)pos.z})};

	game_.EndTurn();

	return true;
}

Window::Window() : screen_width_(800), screen_height_(800), theta_(0),
camera_{ 0.0f, 0.0f, 0.0f }, cameraYaw_(0), cameraPitch_(0), models_(),
selectedCube_{ 1, 1, 1 }, selectedCubes_(), lookDir_{ 0, 0, 0 },
cameraDistance_(30), meshes_(), game_()
{
	// Projection Matrix
	float near = 0.1f;
	float far = 1000.0f;
	float fov = 90.0f;
	float aspect_ratio = (float)screen_height_ / (float)screen_width_;

	projection_matrix_ = util::MakeProjectionMatrix(fov, aspect_ratio, near, far);

	for (int z = 0; z < 3; z++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				std::ostringstream ss;
				ss << "cube_" << x << y << z;
				std::string name = ss.str();
				auto cube = util::MakeUnitCube();
				cube.setColor(BLANK);
				cube.showWireframe(true);
				models_[name] = util::Model{ cube, {-1.5f + x, -1.5f + y, -1.5f + z} };
			}
		}
	}

	SelectCube({ 1, 1, 1 });

	auto xMesh = util::LoadMeshFromObj("x.obj");
	auto oMesh = util::LoadMeshFromObj("o.obj");
	xMesh.setColor(RED);
	oMesh.setColor(BLUE);
	meshes_["X"] = xMesh;
	meshes_["O"] = oMesh;
}

void Window::Open()
{
	InitWindow(screen_width_, screen_height_, "TicTacToe 3D");
	SetTargetFPS(60);

	float elapsed_time = 0.0f;
	double total_time = 0.0;
	double lastUpdateTime = GetTime();

	bool isDragging = false;

	Vector2 lastMousePos = { 0, 0 };
	Vector2 currentMousePos;

	while (!WindowShouldClose()) {
		currentMousePos = GetMousePosition();

		total_time = GetTime();
		elapsed_time = (float)(total_time - lastUpdateTime);
		lastUpdateTime = total_time;

		//if (IsKeyDown(KEY_RIGHT)) cameraYaw_ += 0.5f * elapsed_time;
		//if (IsKeyDown(KEY_LEFT)) cameraYaw_ -= 0.5f * elapsed_time;
		//Vector3 forward = Vector3Scale(lookDir_, 8.0f * elapsed_time);
		//if (IsKeyDown(KEY_UP)) camera_ = Vector3Add(camera_, forward);
		//if (IsKeyDown(KEY_DOWN)) camera_ = Vector3Subtract(camera_, forward);
		//if (IsKeyDown(KEY_Q)) cameraPitch_ += 0.5f * elapsed_time;
		//if (IsKeyDown(KEY_E)) cameraPitch_ -= 0.5f * elapsed_time;

		if (IsKeyDown(KEY_UP)) cameraDistance_ -= 5.f * elapsed_time;
		if (IsKeyDown(KEY_DOWN)) cameraDistance_ += 5.f * elapsed_time;
		
		if (IsKeyPressed(KEY_D)) SelectCube(selectedCube_ + Vec3{ 1, 0, 0 });
		if (IsKeyPressed(KEY_A)) SelectCube(selectedCube_ + Vec3{ -1, 0, 0 });
		if (IsKeyPressed(KEY_W)) SelectCube(selectedCube_ + Vec3{ 0, 0, -1 });
		if (IsKeyPressed(KEY_S)) SelectCube(selectedCube_ + Vec3{ 0, 0, 1 });
		if (IsKeyPressed(KEY_Q)) SelectCube(selectedCube_ + Vec3{ 0, 1, 0 });
		if (IsKeyPressed(KEY_E)) SelectCube(selectedCube_ + Vec3{ 0, -1, 0 });
		if (IsKeyPressed(KEY_SPACE)) useCube(selectedCube_);

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			isDragging = true;
		}
		else {
			isDragging = false;
		}

		if (isDragging) {
			Vector2 deltaMouse = Vector2Subtract(currentMousePos, lastMousePos);
			cameraPitch_ += deltaMouse.y * 0.1f * elapsed_time;
			cameraYaw_ += deltaMouse.x * 0.1f * elapsed_time;
		}

		lastMousePos = currentMousePos;

		Vector3 up = { 0, 1, 0 };
		Vector3 target = { 0, 0, -1 };
		cameraPitch_ = Clamp(cameraPitch_, (-PI / 2) * 0.99f, (PI / 2) * 0.99f);
		Matrix cameraRot = MatrixRotateXYZ({cameraPitch_, cameraYaw_, 0});
		lookDir_ = util::MultiplyMatrixVector3(target, cameraRot);

		camera_ = Vector3Subtract({ 0,0,0 }, Vector3Scale(lookDir_, cameraDistance_));
		target = Vector3Subtract(camera_, lookDir_);
		Matrix cameraMatrix = MatrixLookAt(camera_, target, up);
		Matrix viewMatrix = cameraMatrix;

		std::vector<util::Triangle> triangles_to_raster;

		for (auto& entry : models_) {

			Matrix worldMatrix = MatrixIdentity();
			Matrix transformMatrix;
			if (entry.first.find("flat_") == 0) {
				transformMatrix = util::BillboardMatrix(viewMatrix, entry.second.position);
			}
			else {
				transformMatrix = MatrixTranslate(entry.second.position.x, entry.second.position.y, entry.second.position.z);
			}
			worldMatrix = MatrixMultiply(worldMatrix, transformMatrix);

			// Transform triangles
			for (auto& tri : entry.second.mesh.tris) {

				util::Triangle tri_projected, triTransformed, triViewed;

				triTransformed.p[0] = util::MultiplyMatrixVector3(tri.p[0], worldMatrix);
				triTransformed.p[1] = util::MultiplyMatrixVector3(tri.p[1], worldMatrix);
				triTransformed.p[2] = util::MultiplyMatrixVector3(tri.p[2], worldMatrix);

				// This is where you would check if the triangle needs to be drawn using its normal, but we can just draw everything

				triViewed.p[0] = util::MultiplyMatrixVector3(triTransformed.p[0], viewMatrix);
				triViewed.p[1] = util::MultiplyMatrixVector3(triTransformed.p[1], viewMatrix);
				triViewed.p[2] = util::MultiplyMatrixVector3(triTransformed.p[2], viewMatrix);
				triViewed.color = tri.color;
				triViewed.hide_edge_1 = tri.hide_edge_1;
				triViewed.hide_edge_2 = tri.hide_edge_2;
				triViewed.hide_edge_3 = tri.hide_edge_3;

				// Clip triangles
				int clippedTriangles = 0;
				util::Triangle clipped[2];
				clippedTriangles = util::TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < clippedTriangles; n++) {
					tri_projected.p[0] = util::MultiplyMatrixVector3(clipped[n].p[0], projection_matrix_);
					tri_projected.p[1] = util::MultiplyMatrixVector3(clipped[n].p[1], projection_matrix_);
					tri_projected.p[2] = util::MultiplyMatrixVector3(clipped[n].p[2], projection_matrix_);
					tri_projected.hide_edge_1 = clipped[n].hide_edge_1;
					tri_projected.hide_edge_2 = clipped[n].hide_edge_2;
					tri_projected.hide_edge_3 = clipped[n].hide_edge_3;
					tri_projected.color = clipped[n].color;

					// Scale into view
					Vector3 offsetView = { 1,1,0 };
					tri_projected.p[0] = Vector3Add(tri_projected.p[0], offsetView);
					tri_projected.p[1] = Vector3Add(tri_projected.p[1], offsetView);
					tri_projected.p[2] = Vector3Add(tri_projected.p[2], offsetView);

					tri_projected.p[0].x *= 0.5f * (float)screen_width_; tri_projected.p[0].y *= 0.5f * (float)screen_height_;
					tri_projected.p[1].x *= 0.5f * (float)screen_width_; tri_projected.p[1].y *= 0.5f * (float)screen_height_;
					tri_projected.p[2].x *= 0.5f * (float)screen_width_; tri_projected.p[2].y *= 0.5f * (float)screen_height_;

					triangles_to_raster.push_back(tri_projected);
				}
			}
		}

		sort(triangles_to_raster.begin(), triangles_to_raster.end(), [](util::Triangle& t1, util::Triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			}
		);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("TicTacToe 3D!", 190, 200, 20, LIGHTGRAY);
		DrawFPS(5, 5);
		DrawText(FormatText("Camera pos: %2.02f, %2.02f, %2.02f,", camera_.x, camera_.y, camera_.z), 5, 25, 20, BLACK);

		for (auto& triProjected : triangles_to_raster) {
			DrawTriangle(
				{ triProjected.p[0].x, triProjected.p[0].y },
				{ triProjected.p[1].x, triProjected.p[1].y },
				{ triProjected.p[2].x, triProjected.p[2].y }, triProjected.color);

			if (!triProjected.hide_edge_1) {
				DrawLine(triProjected.p[0].x, triProjected.p[0].y,
					triProjected.p[1].x, triProjected.p[1].y, RED);
			}
			if (!triProjected.hide_edge_2) {
				DrawLine(triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, RED);
			}
			if (!triProjected.hide_edge_3) {
				DrawLine(triProjected.p[2].x, triProjected.p[2].y,
					triProjected.p[0].x, triProjected.p[0].y, RED);
			}
		}

		EndDrawing();
	}

	CloseWindow();
}
