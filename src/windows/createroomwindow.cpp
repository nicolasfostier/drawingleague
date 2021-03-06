#include "include/windows/createroomwindow.h"



// Constructor
CreateRoomWindow::CreateRoomWindow() : QDialog()
{
	connection = NULL;

	server = NULL;

	dictionaryPath = QString();

	layout = new QGridLayout(this);
	layout->setVerticalSpacing(10);

		labelFont.setBold(true);


		pseudoLabel = new QLabel(tr("Your pseudo :"), this);
		pseudoLabel->setFont(labelFont);
		layout->addWidget(pseudoLabel, 0, 0, 1, 1);

		pseudoLineEdit = new QLineEdit(this);
		layout->addWidget(pseudoLineEdit, 0, 1, 1, 2);


		QFrame* line = new QFrame(this);
		line->setFrameShape(QFrame::HLine);
		layout->addWidget(line, 1, 0, 1, 3);


		roomNameLabel = new QLabel(tr("Room name :"), this);
		roomNameLabel->setFont(labelFont);
		layout->addWidget(roomNameLabel, 2, 0, 1, 1);

		roomNameLineEdit = new QLineEdit(this);
		roomNameLineEdit->setText("Drawing League");
		layout->addWidget(roomNameLineEdit, 2, 1, 1, 2);


		portLabel = new QLabel(tr("Server port :"), this);
		portLabel->setFont(labelFont);
		layout->addWidget(portLabel, 3, 0, 1, 1);

		portLineEdit = new QLineEdit(this);
		portLineEdit->setText("23232");
		layout->addWidget(portLineEdit, 3, 1, 1, 2);


		maxPlayersLabel = new QLabel(tr("Maximum number of players :"), this);
		maxPlayersLabel->setFont(labelFont);
		layout->addWidget(maxPlayersLabel, 4, 0, 1, 1);

		maxPlayersLineEdit = new QLineEdit(this);
		maxPlayersLineEdit->setText("10");
		layout->addWidget(maxPlayersLineEdit, 4, 1, 1, 2);


		dictionaryTypeLabel = new QLabel(tr("Dictionary type :"), this);
		dictionaryTypeLabel->setFont(labelFont);
		layout->addWidget(dictionaryTypeLabel, 5, 0, 1, 1);

		dictionaryTypeComboBox = new QComboBox(this);
		dictionaryTypeComboBox->insertItem(0, tr("Standard"));
		dictionaryTypeComboBox->insertItem(1, tr("Custom"));
		layout->addWidget(dictionaryTypeComboBox, 5, 1, 1, 1);

			dictionaryStackedLayout = new QStackedLayout();
			QObject::connect(dictionaryTypeComboBox, SIGNAL(currentIndexChanged(int)),
							 dictionaryStackedLayout, SLOT(setCurrentIndex(int)));
			layout->addLayout(dictionaryStackedLayout, 5, 2, 1, 1);

				dictionaryStandardComboBox = new QComboBox(this);
//                dictionaryStandardComboBox->insertItem(0, tr("Easy english"));
				dictionaryStandardComboBox->insertItem(1, tr("Easy french"));
				dictionaryStackedLayout->addWidget(dictionaryStandardComboBox);

				browseDictionaryButton = new QPushButton("...", this);
				QObject::connect(browseDictionaryButton, SIGNAL(clicked(bool)),
								 this, SLOT(browseDictionary()));
				dictionaryStackedLayout->addWidget(browseDictionaryButton);


		maxRoundLabel = new QLabel(tr("Number of rounds :"), this);
		maxRoundLabel->setFont(labelFont);
		layout->addWidget(maxRoundLabel, 6, 0, 1, 1);

		maxRoundLineEdit = new QLineEdit(this);
		maxRoundLineEdit->setText("10");
		layout->addWidget(maxRoundLineEdit, 6, 1, 1, 2);


		timeByRoundLabel = new QLabel(tr("Time by round (in second) :"), this);
		timeByRoundLabel->setFont(labelFont);
		layout->addWidget(timeByRoundLabel, 7, 0, 1, 1);

		timeByRoundLineEdit = new QLineEdit(this);
		timeByRoundLineEdit->setText("180");
		layout->addWidget(timeByRoundLineEdit, 7, 1, 1, 2);


		timeAfterFirstGoodAnswerLabel = new QLabel(tr("Time after the first good answer (in second) :"), this);
		timeAfterFirstGoodAnswerLabel->setFont(labelFont);
		layout->addWidget(timeAfterFirstGoodAnswerLabel, 8, 0, 1, 1);

		timeAfterFirstGoodAnswerLineEdit = new QLineEdit(this);
		timeAfterFirstGoodAnswerLineEdit->setText("30");
		layout->addWidget(timeAfterFirstGoodAnswerLineEdit, 8, 1, 1, 2);


		createOrCancel = new QWidget(this);
		layout->addWidget(createOrCancel, 9, 0, 1, 3, Qt::AlignHCenter);

			layoutConnectOrCancel = new QHBoxLayout(createOrCancel);

				cancelButton = new QPushButton(tr("Cancel"), this);
				QObject::connect(cancelButton, SIGNAL(clicked(bool)),
								 this, SLOT(close()));
				layoutConnectOrCancel->addWidget(cancelButton);

				createButton = new QPushButton(tr("Create"), this);
				createButton->setDefault(true);
				QObject::connect(createButton, SIGNAL(clicked(bool)),
								 this, SLOT(createRoom()));
				layoutConnectOrCancel->addWidget(createButton);


	this->setWindowTitle(tr("Create a room"));
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
}



// Destructor
CreateRoomWindow::~CreateRoomWindow(){
	if(connection != NULL && !connection->getIsConnected()){
		connection->deleteLater();
		connection = NULL;

		server->deleteLater();
		server = NULL;
	}
}



// Qt slots

void CreateRoomWindow::browseDictionary(){
	QString newDictionaryPath = QFileDialog::getOpenFileName(this,
															 tr("Choose your dictionary"),
															 QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(),
																					QStandardPaths::LocateDirectory),
															 tr("Text files") +" (*.txt)");
	if(!newDictionaryPath.isEmpty()){
		dictionaryPath = newDictionaryPath;
		browseDictionaryButton->setText(QString(QFileInfo(dictionaryPath).fileName()));
	}
}


void CreateRoomWindow::createRoom(){
	if(dictionaryTypeComboBox->currentText() == tr("Standard")){
		if(dictionaryStandardComboBox->currentText() == tr("Easy english")){
			dictionaryPath = ":/dictionaries/easy_english.txt";
		}
		else if(dictionaryStandardComboBox->currentText() == tr("Easy french")){
			dictionaryPath = ":/dictionaries/easy_french.txt";
		}
	}

	try{
		Room room(roomNameLineEdit->text(),
				  maxRoundLineEdit->text().toInt(),
				  maxPlayersLineEdit->text().toInt(),
				  timeByRoundLineEdit->text().toInt(),
				  timeAfterFirstGoodAnswerLineEdit->text().toInt());
		if(this->server != NULL){
			delete this->server;
			this->server = NULL;
		}
		this->server = new Server(portLineEdit->text().toInt(), room, dictionaryPath);
		QObject::connect(this->server, SIGNAL(isReady()),
						 this, SLOT(newConnection()));
		this->server->thread()->start();
	}
	catch(std::invalid_argument exception){
		QMessageBox::critical(this, tr("Creation failed"), exception.what());
	}
}



void CreateRoomWindow::newConnection(){
	if(connection != NULL){
		delete connection;
		connection = NULL;
	}

	connection = new Connection(QHostAddress(QHostAddress::LocalHost).toString(),
								this->portLineEdit->text(),
								this->pseudoLineEdit->text());
	QObject::connect(connection, SIGNAL(hasEnteredTheGame()),
					 this, SLOT(hasEnteredTheGame()));

	connection->connectToTheServer();
}

void CreateRoomWindow::hasEnteredTheGame(){
	this->connection->setIsConnected(true);

	emit roomCreated(server, connection);

	this->close();
}
