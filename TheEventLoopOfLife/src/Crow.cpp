#include "Crow.h"

Crow::Crow(sf::Vector2i _pos)
{
	pos = _pos;
	posf = sf::Vector2f(pos.x, pos.y);
	minVelocity = sf::Vector2f(
		float(rand() / (RAND_MAX + 1.f)) * 2.0f - 1.0f,
		float(rand() / (RAND_MAX + 1.f)) * 2.0f - 1.0f
	);
	uint32_t tileSize = Grid::Instance()->TileSize();
	size = tileSize * 0.2f;
	body.setRadius(size);
	body.setScale(sf::Vector2f(0.7f, 1.0f));
	body.setOutlineThickness(2.0f);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setFillColor(sf::Color::Magenta);

	shadow.setRadius(size);
	shadow.setScale(sf::Vector2f(0.7f, 1.0f));
	shadow.setOrigin(size, size);
	shadow.setFillColor(sf::Color(30, 30, 30, 60));
}

void Crow::update(std::vector<Crow*>& _crows)
{
	alignmentCrows = getCrowsInRadius(_crows, alignmentRadius);
	cohesionCrows = getCrowsInRadius(_crows, cohesionRadius);
	separationCrows = getCrowsInRadius(_crows, separationRadius);

	sf::Vector2f idealVelocity = sf::Vector2f(0.0f, 0.0f);
	alignment = normalize(calcAlignment());
	cohesion = normalize(calcCohesion());
	separation = normalize(calcSeparation());
	idealVelocity = alignment + cohesion + separation;
	idealVelocity = normalize(idealVelocity);
	velocity = velocity * (1.0f - steerPercentage) + idealVelocity * steerPercentage;
	if (magnitude(velocity) < magnitude(minVelocity)) {
		velocity = minVelocity;
	}
	posf += velocity * Time::deltaTime * speed;
	uint32_t tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);

	double pi = 2 * acos(0.0);
	float rotation = (float)(atan2(velocity.y, velocity.x) * 180.f / pi + 90.f);
	body.setRotation(rotation);

	shadow.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2 + shadowOffset * tileSize);
	shadow.setRotation(rotation);

	worldWrap();
}

sf::CircleShape Crow::getBody()
{
	return body;
}

sf::CircleShape Crow::getShadow()
{
	return shadow;
}

sf::Vector2f Crow::calcAlignment()
{
	sf::Vector2f result = sf::Vector2f(0.0f, 0.0f);
	for (auto crow = alignmentCrows.begin(); crow != alignmentCrows.end(); crow++) {
		result += (*crow)->velocity;
	}
	return result;
}

sf::Vector2f Crow::calcCohesion()
{
	sf::Vector2f result = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f centerOfMass = sf::Vector2f(0.0f, 0.0f);
	for (auto crow = cohesionCrows.begin(); crow != cohesionCrows.end(); crow++) {
		centerOfMass += (*crow)->posf;
	}
	if (cohesionCrows.size() > 0) {
		centerOfMass.x /= cohesionCrows.size();
		centerOfMass.y /= cohesionCrows.size();
		result = centerOfMass - posf;
		return result;
	}
	return sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f Crow::calcSeparation()
{
	sf::Vector2f result = sf::Vector2f(0.0f, 0.0f);
	for (auto crow = separationCrows.begin(); crow != separationCrows.end(); crow++) {
		result += (*crow)->posf - posf;
	}
	result = invert(result);
	return result;
}

std::vector<Crow*> Crow::getCrowsInRadius(std::vector<Crow*>& _crows, float _radius)
{
	std::vector<Crow*> result;
	for (auto crow = _crows.begin(); crow != _crows.end(); crow++) {
		auto crowPos = (*crow)->posf;
		sf::Vector2f distanceVec = crowPos - posf;
		if (magnitude(distanceVec) != 0.0f) {
			float distance = magnitude(distanceVec);
			if (distance < _radius) {
				result.push_back(*crow);
			}
		}
	}
	return result;
}

void Crow::worldWrap()
{
	if (posf.x < -1.0f)
		posf.x = Grid::Instance()->Columns();
	if (posf.x > Grid::Instance()->Columns())
		posf.x = -1.0f;
	if (posf.y < -1.0f)
		posf.y = Grid::Instance()->Rows();
	if (posf.y > Grid::Instance()->Rows())
		posf.y = -1.0f;
}
