#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(250, 2);
	auto triangles = ico_sphere.getMesh().getUniqueFaces();

	for (auto& triangle : triangles) {

		auto avg = glm::vec3(triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		this->base_vertices.push_back(avg);
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	auto param = glm::vec3(3.5, 0, 0);
	for (auto& vertex : this->base_vertices) {

		int start_index = this->frame.getNumVertices();
		for (int i = 0; i < 20; i++) {

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.005), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.005), 0, 1, -PI, PI), glm::vec3(0, 1, 0));

			auto next_rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
			auto next_rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -PI, PI), glm::vec3(0, 1, 0));

			glm::vec3 tmp = glm::vec4(vertex, 0) * rotation_y * rotation_x;
			glm::vec3 next_tmp = glm::vec4(vertex, 0) * next_rotation_y * next_rotation_x;

			glm::vec3 direction = next_tmp - tmp;
			auto theta = atan2(direction.y, direction.x);
			auto tmp_rotation_x = glm::rotate(glm::mat4(), (float)(PI * 0.5), glm::vec3(1, 0, 0));
			auto tmp_rotation_y = glm::rotate(glm::mat4(), (float)(theta + PI * 0.5), glm::vec3(0, 1, 0));
			glm::vec3 tmp_param = glm::vec4(param, 0) * tmp_rotation_y * tmp_rotation_x;

			int face_index = this->face.getNumVertices() - 2;
			int frame_index = this->frame.getNumVertices() - 2;

			this->face.addVertex(tmp + tmp_param);
			this->face.addVertex(tmp - tmp_param);

			this->frame.addVertex(tmp + tmp_param);
			this->frame.addVertex(tmp - tmp_param);
			
			if (i > 0) {

				this->face.addIndex(face_index + 0); this->face.addIndex(face_index + 1); this->face.addIndex(face_index + 3);
				this->face.addIndex(face_index + 0); this->face.addIndex(face_index + 3); this->face.addIndex(face_index + 2);

				this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index + 2);
				this->frame.addIndex(frame_index + 1); this->frame.addIndex(frame_index + 3);
			}
		}

		this->frame.addIndex(start_index + 0); this->frame.addIndex(start_index + 1);
		this->frame.addIndex(frame.getNumVertices() - 1); this->frame.addIndex(frame.getNumVertices() - 2);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}