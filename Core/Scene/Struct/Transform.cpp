#include <Scene/Struct/Transform.h>
glm::mat4 XVerse::XTransform::ModelMatrix()const
{
	glm::mat4 Model(1.0f);
	Model = glm::scale(Model, LocalScale);

	float roll = -LocalRotation.x;
	float pitch = -LocalRotation.y;
	float yaw = LocalRotation.z;
	glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(roll, pitch, yaw)));
	Model = glm::mat4_cast(rotationQuat) * Model;
	Model = glm::translate(Model, LocalPosition);
	return Model;
}

glm::mat4 XVerse::XTransform::ViewMatrix()const
{
	return glm::lookAt(LocalPosition, LocalPosition + Front, Up);
}

glm::mat4 XVerse::XTransform::LookAtMatrix(const glm::vec3& LookAtPos)const
{
	return glm::lookAt(LocalPosition, LookAtPos, WorldUp());
};


void XVerse::XTransform::LookAtPX()
{
	SetRotation({ 0,0,0 });
}

void XVerse::XTransform::LookAtNX()
{
	SetRotation({ 0,0,-180 });
}

void XVerse::XTransform::LookAtPY()
{
	SetRotation({ 0,0,90 });
}

void XVerse::XTransform::LookAtNY()
{
	SetRotation({ 0,0,-90 });
}

void XVerse::XTransform::LookAtPZ()
{
	SetRotation({ 0,90,0 });
}

void XVerse::XTransform::LookAtNZ()
{
	SetRotation({ 0,-90,0 });
}

void XVerse::XTransform::MoveForward(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition += Front * velocity;
}

void XVerse::XTransform::MoveBackward(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition -= Front * velocity;
}

void XVerse::XTransform::MoveLeft(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition -= Right * velocity;
}

void XVerse::XTransform::MoveRight(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition += Right * velocity;
}

void XVerse::XTransform::MoveUp(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition += Up * velocity;
}

void XVerse::XTransform::MoveDown(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	LocalPosition -= Up * velocity;
}
void XVerse::XTransform::Rotate(float deltax, float deltay, float deltaz, bool constrainYaw)
{
	LocalRotation.x += deltax * MouseSensitivity;
	LocalRotation.y += deltay * MouseSensitivity;
	LocalRotation.z += deltaz * MouseSensitivity;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainYaw)
	{
		if (LocalRotation.y > 89.0f)
			LocalRotation.y = 89.0f;
		if (LocalRotation.y < -89.0f)
			LocalRotation.y = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateLocalVectors();
}

bool XVerse::XTransform::Parse(const json11::Json& In)
{
	MovementSpeed = In["MovementSpeed"].number_value();
	MouseSensitivity = In["MouseSensitivity"].number_value();

	LocalPosition.x = In["X"].number_value();
	LocalPosition.y = In["Y"].number_value();
	LocalPosition.z = In["Z"].number_value();

	LocalRotation.x = In["Row"].number_value();
	LocalRotation.y = In["Pitch"].number_value();
	LocalRotation.z = In["Yaw"].number_value();

	LocalScale.x = In["ScaleX"].number_value();
	LocalScale.y = In["ScaleY"].number_value();
	LocalScale.z = In["ScaleZ"].number_value();

	UpdateLocalVectors();
	return true;
}

bool XVerse::XTransform::Serialize(json11::Json& Out)
{
	Out = json11::Json::object
	{
		{ "MovementSpeed", MovementSpeed},
		{ "MouseSensitivity", MouseSensitivity },
		{ "X", LocalPosition.x},
		{ "Y", LocalPosition.y},
		{ "Z", LocalPosition.z},
		{ "Row", LocalRotation.x},
		{ "Pitch", LocalRotation.y},
		{ "Yaw", LocalRotation.z},
		{ "ScaleX", LocalScale.x},
		{ "ScaleY", LocalScale.y},
		{ "ScaleZ", LocalScale.z},
	};
	return true;
}


void XVerse::XTransform::UpdateLocalVectors()
{
	glm::quat rotationQuat = glm::quat(glm::radians(LocalRotation));

	glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

	Front = glm::vec3(rotationMatrix * glm::vec4(WorldFront(), 0.0f));
	Up = glm::vec3(rotationMatrix * glm::vec4(WorldUp(), 0.0f));
	Right = glm::vec3(rotationMatrix * glm::vec4(WorldRight(), 0.0f));
}