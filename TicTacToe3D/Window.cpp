#include "Window.h"
#include "raylib.h"
#include <raymath.h>
#include <algorithm>
#include <sstream>

const std::map<Window::Direction, Vec3> Window::directionMap_ = {
	{Direction::FORWARD, {0, 0, -1}},
	{Direction::RIGHT, {1, 0, 0}},
	{Direction::BACK, {0, 0, 1}},
	{Direction::LEFT, {-1, 0, 0}},
};

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

	auto playerSymbol = game_.GetCurrentPlayer().GetSymbol();

	std::string playerSymbolStr{ playerSymbol };

	models_[modelName] = util::Model{ meshes_[playerSymbolStr], Vector3Add({-1, -1, -1}, {(float)pos.x, (float)pos.y, (float)pos.z})};

	game_.SetCell(pos.x, pos.y, pos.z, playerSymbol);

	game_.EndTurn();

	return true;
}

std::string Window::GetWinnerString() const
{
	std::ostringstream ss;

	auto xPlayer = game_.GetPlayer('X');
	auto oPlayer = game_.GetPlayer('O');

	if (xPlayer.GetPoints() == oPlayer.GetPoints()) {
		ss << "TIE";
	}
	else if (xPlayer.GetPoints() > oPlayer.GetPoints()) {
		ss << xPlayer.GetSymbol() << " WINS!";
	}
	else {
		ss << oPlayer.GetSymbol() << " WINS!";
	}
	
	return ss.str();
}

Window::Window() : screenWidth_(800), screenHeight_(800), theta_(0),
camera_{ 0.0f, 0.0f, 0.0f }, cameraYaw_(0), cameraPitch_(0), models_(),
selectedCube_{ 1, 1, 1 }, selectedCubes_(), lookDir_{ 0, 0, 0 },
cameraDistance_(30), meshes_(), game_()
{
	// Projection Matrix
	float near = 0.1f;
	float far = 1000.0f;
	float fov = 90.0f;
	float aspect_ratio = (float)screenHeight_ / (float)screenWidth_;

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

Vector2 Window::GetCenteredTextPos(const char* text, const int size) const
{
	Vector2 out{};
	out.x = (screenWidth_ - MeasureText(text, size)) / 2.0f;
	out.y = (screenHeight_ - size) / 2.0f;
	return out;
}

void Window::Open()
{
	InitWindow(screenWidth_, screenHeight_, "TicTacToe 3D");
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

		//if (IsKeyDown(KEY_UP)) cameraDistance_ -= 5.f * elapsed_time;
		//if (IsKeyDown(KEY_DOWN)) cameraDistance_ += 5.f * elapsed_time;

		Vec3 frontDir, rightDir, backDir, leftDir;

		Direction startDir = Direction::FORWARD;

		if (cameraYaw_ > (PI / 4.f) && cameraYaw_ <= ((PI * 3.f) / 4.f)) {
			startDir = Direction::RIGHT;
		}
		else if (cameraYaw_ > ((PI * 3.f) / 4.f) && cameraYaw_ <= ((PI * 5.f) / 4.f)) {
			startDir = Direction::BACK;
		}
		else if (cameraYaw_ > ((PI * 5.f) / 4.f) && cameraYaw_ <= ((PI * 7.f) / 4.f)) {
			startDir = Direction::LEFT;
		}


		frontDir = directionMap_.at(startDir);
		rightDir = directionMap_.at(static_cast<Direction>((static_cast<int>(startDir) + 1) % 4));
		backDir = directionMap_.at(static_cast<Direction>((static_cast<int>(startDir) + 2) % 4));
		leftDir = directionMap_.at(static_cast<Direction>((static_cast<int>(startDir) + 3) % 4));


		if (IsKeyDown(KEY_UP)) cameraPitch_ += 5.f * elapsed_time;
		if (IsKeyDown(KEY_DOWN)) cameraPitch_ -= 5.f * elapsed_time;
		if (IsKeyDown(KEY_RIGHT)) cameraYaw_ -= 5.f * elapsed_time;
		if (IsKeyDown(KEY_LEFT)) cameraYaw_ += 5.f * elapsed_time;
		
		if (IsKeyPressed(KEY_D)) SelectCube(selectedCube_ + rightDir);
		if (IsKeyPressed(KEY_A)) SelectCube(selectedCube_ + leftDir);
		if (IsKeyPressed(KEY_W)) SelectCube(selectedCube_ + frontDir);
		if (IsKeyPressed(KEY_S)) SelectCube(selectedCube_ + backDir);
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
		if (cameraYaw_ < 0) {
			cameraYaw_ += 2.0f * PI;
		}
		if (cameraYaw_ >= 2.0f * PI) {
			cameraYaw_ -= 2.0f * PI;
		}
		Matrix cameraRot = MatrixRotateXYZ({cameraPitch_, cameraYaw_, 0});
		lookDir_ = util::MultiplyMatrixVector3(cameraRot, target);

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

				triTransformed.p[0] = util::MultiplyMatrixVector3(worldMatrix, tri.p[0]);
				triTransformed.p[1] = util::MultiplyMatrixVector3(worldMatrix, tri.p[1]);
				triTransformed.p[2] = util::MultiplyMatrixVector3(worldMatrix, tri.p[2]);

				// This is where you would check if the triangle needs to be drawn using its normal, but we can just draw everything

				triViewed.p[0] = util::MultiplyMatrixVector3(viewMatrix, triTransformed.p[0]);
				triViewed.p[1] = util::MultiplyMatrixVector3(viewMatrix, triTransformed.p[1]);
				triViewed.p[2] = util::MultiplyMatrixVector3(viewMatrix, triTransformed.p[2]);
				triViewed.color = tri.color;
				triViewed.hide_edge_1 = tri.hide_edge_1;
				triViewed.hide_edge_2 = tri.hide_edge_2;
				triViewed.hide_edge_3 = tri.hide_edge_3;

				// Clip triangles
				int clippedTriangles = 0;
				util::Triangle clipped[2];
				clippedTriangles = util::TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < clippedTriangles; n++) {
					tri_projected.p[0] = util::MultiplyMatrixVector3(projection_matrix_, clipped[n].p[0]);
					tri_projected.p[1] = util::MultiplyMatrixVector3(projection_matrix_, clipped[n].p[1]);
					tri_projected.p[2] = util::MultiplyMatrixVector3(projection_matrix_, clipped[n].p[2]);
					tri_projected.hide_edge_1 = clipped[n].hide_edge_1;
					tri_projected.hide_edge_2 = clipped[n].hide_edge_2;
					tri_projected.hide_edge_3 = clipped[n].hide_edge_3;
					tri_projected.color = clipped[n].color;

					// Scale into view
					Vector3 offsetView = { 1,1,0 };
					tri_projected.p[0] = Vector3Add(tri_projected.p[0], offsetView);
					tri_projected.p[1] = Vector3Add(tri_projected.p[1], offsetView);
					tri_projected.p[2] = Vector3Add(tri_projected.p[2], offsetView);

					tri_projected.p[0].x *= 0.5f * (float)screenWidth_; tri_projected.p[0].y *= 0.5f * (float)screenHeight_;
					tri_projected.p[1].x *= 0.5f * (float)screenWidth_; tri_projected.p[1].y *= 0.5f * (float)screenHeight_;
					tri_projected.p[2].x *= 0.5f * (float)screenWidth_; tri_projected.p[2].y *= 0.5f * (float)screenHeight_;

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
		DrawText(FormatText("Camera pos: %2.02f, %2.02f, %2.02f", camera_.x, camera_.y, camera_.z), 5, 25, 20, BLACK);

		auto xPlayerScore = game_.GetPlayer('X').GetPoints();
		auto oPlayerScore = game_.GetPlayer('O').GetPoints();
		auto scoreText = FormatText("%d :X | O: %d", xPlayerScore, oPlayerScore);
		auto scoreTextSize = 40;
		auto scoreXPos = screenWidth_ / 2.0f - MeasureText(scoreText, scoreTextSize)/2.0f;
		DrawText(scoreText, scoreXPos, 30, scoreTextSize, BLACK);

		for (auto& triProjected : triangles_to_raster) {
			DrawTriangle(
				{ triProjected.p[0].x, triProjected.p[0].y },
				{ triProjected.p[1].x, triProjected.p[1].y },
				{ triProjected.p[2].x, triProjected.p[2].y }, triProjected.color);

			if (!triProjected.hide_edge_1) {
				DrawLine(triProjected.p[0].x, triProjected.p[0].y,
					triProjected.p[1].x, triProjected.p[1].y, BLACK);
			}
			if (!triProjected.hide_edge_2) {
				DrawLine(triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, BLACK);
			}
			if (!triProjected.hide_edge_3) {
				DrawLine(triProjected.p[2].x, triProjected.p[2].y,
					triProjected.p[0].x, triProjected.p[0].y, BLACK);
			}
		}

		if (game_.PossibleMovesLeft()) {
			auto turnText = FormatText("Turn of: %c", game_.GetCurrentPlayer().GetSymbol());
			DrawText(turnText, 5, 25, 20, BLACK);
		}
		else {
			auto gameOverText = "GAME OVER";
			auto gameOverTextSize = 50;
			auto gameOverTextXPos = (screenWidth_ - MeasureText(gameOverText, gameOverTextSize)) / 2.0f;
			auto gameOverTextYPos = (screenHeight_ - gameOverTextSize) / 2.0f;
			DrawText(gameOverText, gameOverTextXPos, gameOverTextYPos, gameOverTextSize, BLACK);
			auto winnerTextStr = GetWinnerString();
			char winnerText[64]{};
			winnerTextStr.copy(winnerText, winnerTextStr.size() + 1);
			auto winnerTextPos = GetCenteredTextPos(winnerText, gameOverTextSize);
			DrawText(winnerText, winnerTextPos.x, gameOverTextYPos + gameOverTextSize, gameOverTextSize, BLACK);
		}

		EndDrawing();
	}

	CloseWindow();
}
