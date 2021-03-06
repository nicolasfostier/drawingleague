#include "include/gameinfo/player.h"



// Getter
QString Player::getPseudo(){
	return pseudo->text();
}
int Player::getScore(){
	return score->text().toInt();
}
bool Player::getIsReady(){
	return isReady;
}
bool Player::getAnswerFound(){
	return answerFound;
}
bool Player::getIsArtist(){
	return isArtist;
}



// Setter
void Player::setScore(int score){
	this->score->setText(QString::number(score));
}
void Player::setIsReady(bool isReady){
	this->isReady = isReady;
}
void Player::setAnswerFound(bool answerFound){
	this->answerFound = answerFound;
}
void Player::setIsArtist(bool isArtist){
	this->isArtist = isArtist;
}



// Constructor
Player::Player(QString pseudo, int score){
	this->pseudo = new QTableWidgetItem(pseudo);
	this->pseudo->setFlags(Qt::ItemIsEnabled);

	this->score = new PlayerScore(QString::number(score));
	this->score->setFlags(Qt::ItemIsEnabled);

	this->isReady = false;

	this->answerFound = false;
	this->isArtist = false;
}
Player::Player(const Player &player){
	this->pseudo = new QTableWidgetItem(*player.pseudo);
	this->pseudo->setFlags(Qt::ItemIsEnabled);

	this->score = new PlayerScore(*player.score);
	this->score->setFlags(Qt::ItemIsEnabled);

	this->isReady = player.isReady;

	this->answerFound = player.answerFound;
	this->isArtist = player.isArtist;
}



// Destructor
Player::~Player(){
	delete pseudo;
	delete score;
}



// Methods

void Player::pseudoToHTMLEscaped(){
	this->pseudo->setText(this->pseudo->text().toHtmlEscaped());
}

void Player::addToTableWidget(QTableWidget *playersTable){
	playersTable->setRowCount(playersTable->rowCount() + 1);
	playersTable->setItem(playersTable->rowCount()-1, 0, this->pseudo);
	playersTable->setItem(playersTable->rowCount()-1, 1, this->score);
	playersTable->sortItems(0, Qt::AscendingOrder);
	playersTable->sortItems(1, Qt::DescendingOrder);
}

void Player::updateColor(){
	QFont font;
	if(isArtist){
		this->pseudo->setBackgroundColor(QColor("#87d687"));
		this->score->setBackgroundColor(QColor("#87d687"));
		font.setBold(true);
		this->pseudo->setFont(font);
	}
	else if(answerFound){
		this->pseudo->setBackgroundColor(QColor("#de4d4d"));
		this->score->setBackgroundColor(QColor("#de4d4d"));
		font.setBold(true);
		this->pseudo->setFont(font);
	}
	else{
		this->pseudo->setBackgroundColor(QColor(Qt::white));
		this->score->setBackgroundColor(QColor(Qt::white));
		font.setBold(false);
		this->pseudo->setFont(font);
	}
}



// Operators
QDataStream& operator<<(QDataStream& dataStream, Player player){
	dataStream << *(player.pseudo);
	dataStream << *(player.score);
	dataStream << player.isReady;
	dataStream << player.answerFound;
	dataStream << player.isArtist;

	return dataStream;
}
QDataStream& operator>>(QDataStream& dataStream, Player& player){
	dataStream >> *(player.pseudo);
	dataStream >> *(player.score);
	dataStream >> player.isReady;
	dataStream >> player.answerFound;
	dataStream >> player.isArtist;

	return dataStream;
}
