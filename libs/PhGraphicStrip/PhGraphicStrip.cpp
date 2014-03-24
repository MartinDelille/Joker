/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphicStrip.h"

PhGraphicStrip::PhGraphicStrip(QObject *parent) :
	QObject(parent),
	_doc(this),
	_clock(_doc.getTCType()),
	_trackNumber(4),
	_settings(NULL),
	_maxDrawElapsed(0),
	_dropDetected(0)
{
	// update the  content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(onDocChanged()));
}

PhStripDoc *PhGraphicStrip::doc()
{
	return &_doc;
}

PhClock *PhGraphicStrip::clock()
{
	return &_clock;
}

void PhGraphicStrip::setSettings(PhGraphicStripSettings *settings)
{
	PHDEBUG;
	_settings = settings;
}

bool PhGraphicStrip::setFontFile(QString fontFile)
{
	if(_textFont.setFontFile(fontFile)) {
		if(_settings)
			_settings->setTextFontFile(fontFile);
		return true;
	}
	return false;
}

bool PhGraphicStrip::init()
{
	PHDEBUG << _settings;

	PHDEBUG << "Load the strip background";
	_stripBackgroundImage.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png");
	_stripBackgroundImage.init();

	_stripBackgroundImageInverted.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png");
	_stripBackgroundImageInverted.init();

	PHDEBUG << "Init the sync bar";
	_stripSyncBar.setColor(QColor(225, 86, 108));

	PHDEBUG << "Load the font file";
	QString fontFile = "";
	if(_settings != NULL)
		fontFile = _settings->textFontFile();
	else
		PHDEBUG << "no settings...";

	if(!QFile(fontFile).exists()) {
		PHDEBUG << "File not found:" << fontFile;
		fontFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "SWENSON.TTF";
		if(_settings != NULL)
			_settings->textFontFile();
		else
			PHDEBUG << "no settings...";
	}
	_textFont.setFontFile(fontFile);

	if(_settings != NULL)
		_textFont.setBoldness(_settings->textBoldness());

	// Init the sync bar
	_stripSyncBar.setColor(QColor(225, 86, 108));

	_hudFont.setFontFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "ARIAL.TTF");

	// This is used to make some time-based test
	_testTimer.start();

	return true;
}

void PhGraphicStrip::onDocChanged()
{
	foreach(PhGraphicText * gPeople, _graphicPeoples.values())
	delete gPeople;
	_graphicPeoples.clear();

	foreach(PhGraphicSolidRect * gCut, _graphicCuts.values())
	delete gCut;
	_graphicCuts.clear();

	foreach(PhGraphicText * gText, _graphicTexts.values())
	delete gText;
	_graphicTexts.clear();

	foreach(PhGraphicLoop * gLoop, _graphicLoops.values())
	delete gLoop;
	_graphicLoops.clear();

	foreach(PhGraphicRect * gDetect, _graphicDetects.values())
	delete gDetect;
	_graphicDetects.clear();

	_trackNumber = 4;
	foreach(PhStripText *text, _doc.getTexts()) {
		if(text->getTrack() >= _trackNumber)
			_trackNumber = text->getTrack() + 1;
	}

	foreach(PhStripDetect *detect, _doc.getDetects()) {
		if(detect->getTrack() >= _trackNumber)
			_trackNumber = detect->getTrack() + 1;
	}
}

PhFont *PhGraphicStrip::getTextFont()
{
	return &_textFont;
}

PhFont *PhGraphicStrip::getHUDFont()
{
	return &_hudFont;
}

QColor PhGraphicStrip::computeColor(PhPeople * people, QList<PhPeople*> selectedPeoples, bool invertColor)
{
	if(!invertColor) {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(100, 100, 100);
			}
			else {
				return people->getColor();
			}
		}
		else if(selectedPeoples.size())
			return QColor(100, 100, 100);
		else
			return Qt::black;
	}
	else {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(155, 155, 155);
			}
			else {
				QColor color(people->getColor());
				return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
			}
		}
		else if(selectedPeoples.size())
			return QColor(155, 155, 155);
		else
			return Qt::white;
	}
}

void PhGraphicStrip::draw(int x, int y, int width, int height, QList<PhPeople *> selectedPeoples)
{
	int counter = 0;
	bool invertedColor = _settings->invertColor();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int lastDrawElapsed = _testTimer.elapsed();
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();

	if(height > 0) {
		int pixelPerFrame = _settings->horizontalSpeed();
		_textFont.setBoldness(_settings->textBoldness());
		_textFont.setFontFile(_settings->textFontFile());

		int loopCounter = 0;
		int offCounter = 0;
		int cutCounter = 0;

		int fps = PhTimeCode::getFps(_clock.timeCodeType());
		long syncBar_X_FromLeft = width / 6;
		long offset = _clock.time() * pixelPerFrame * fps / _clock.timeScale() - syncBar_X_FromLeft;
		long delay = (int)(_settings->screenDelay() * _clock.rate()); // delay in ms
		// add the delay to the offset
		offset += delay * pixelPerFrame * fps / 1000;
		//Compute the visible duration of the strip
		PhFrame stripDuration = width / pixelPerFrame;

		PhFrame clockFrame = _clock.frame() + delay * fps / 1000;

		if(_settings->stripTestMode()) {
			foreach(PhStripCut * cut, _doc.getCuts())
			{
				counter++;
				if(cut->getTimeIn() == clockFrame) {
					PhGraphicSolidRect white(x, y, width, height);
					white.setColor(Qt::white);
					white.draw();
					PHDEBUG << clockFrame << "cut" << _clock.frame();

					//This is useless to continue the foreach if the cut is displayed.
					break;
				}
			}
			return;
		}

		PhFrame frameIn = clockFrame - syncBar_X_FromLeft / pixelPerFrame;
		PhFrame frameOut = frameIn + stripDuration;

		//Draw backgroung picture
		int n = width / height + 2; // compute how much background repetition do we need
		long leftBG = 0;
		if(offset >= 0)
			leftBG -= offset % height;
		else
			leftBG -= height - ((-offset) % height);

		PhGraphicTexturedRect* backgroundImage = &_stripBackgroundImage;
		if(invertedColor)
			backgroundImage = &_stripBackgroundImageInverted;

		backgroundImage->setX(x + leftBG);
		backgroundImage->setY(y);
		backgroundImage->setSize(height * n, height);
		backgroundImage->setZ(-2);
		backgroundImage->setTextureCoordinate(n, 1);
		backgroundImage->draw();

		_stripSyncBar.setSize(4, height);
		_stripSyncBar.setPosition(x + width/6, y, 0);
		_stripSyncBar.setColor(QColor(225, 86, 108));

		_stripSyncBar.draw();

		if(_settings->displayRuler()) {
			PhFrame rulerTimestamp = _settings->rulerTimestamp();
			PhFrame spaceBetweenRuler = _settings->spaceBetweenRuler();
			int rulerNumber = (frameIn - rulerTimestamp) / spaceBetweenRuler;
			if (rulerNumber < 0)
				rulerNumber = 0;

			PhFrame rulerFrame = rulerTimestamp + rulerNumber * spaceBetweenRuler;
			PhGraphicSolidRect rulerRect;
			PhGraphicDisc rulerDisc;
			PhGraphicText rulerText(&_hudFont);
			QColor rulerColor(80, 80, 80);
			if(invertedColor)
				rulerColor = Qt::white;

			int width = pixelPerFrame;

			rulerRect.setColor(rulerColor);
			rulerRect.setWidth(width);
			rulerRect.setHeight(height / 2);
			rulerRect.setZ(0);
			rulerRect.setY(y);

			rulerDisc.setColor(rulerColor);
			rulerDisc.setRadius(2 * width);
			rulerDisc.setY(y + height / 2 + 3 * width);
			rulerDisc.setZ(0);

			rulerText.setColor(rulerColor);
			rulerText.setY(y + height / 2);
			rulerText.setHeight(height / 2);
			rulerText.setZ(0);


			while (rulerFrame < frameOut + spaceBetweenRuler) {
				counter++;
				int x = rulerFrame * pixelPerFrame - offset;

				rulerRect.setX(x - rulerRect.getWidth() / 2);
				rulerRect.draw();

				QString text = QString::number(rulerNumber);
				rulerText.setContent(text);
				int textWidth = _hudFont.getNominalWidth(text);
				rulerText.setWidth(textWidth);
				rulerText.setX(x - textWidth / 2);
				rulerText.draw();

				x += spaceBetweenRuler * pixelPerFrame / 2;

				rulerRect.setX(x - rulerRect.getWidth() / 2);
				rulerRect.draw();

				rulerDisc.setX(x);
				rulerDisc.draw();

				rulerNumber++;
				rulerFrame += spaceBetweenRuler;
			}
		}

		int minSpaceBetweenPeople = 50;
		int spaceBetweenPeopleAndText = 4;
		PhStripText ** lastTextList = new PhStripText*[_trackNumber];
		for(int i = 0; i < _trackNumber; i++)
			lastTextList[i] = NULL;

		int trackHeight = height / _trackNumber;

		bool trackFull[_trackNumber];
		for(int i = 0; i < _trackNumber; i++) {
			trackFull[i] = false;
		}

		int verticalPixelPerFrame = _settings->verticalSpeed();
		bool displayNextText = _settings->displayNextText();
		PhFrame maxFrameIn = frameOut;
		if(displayNextText)
			maxFrameIn += y / verticalPixelPerFrame;

		foreach(PhStripText * text, _doc.getTexts())
		{
			if(text->getTimeOut() < frameIn)
				continue;
			counter++;
			PhGraphicText* gText = _graphicTexts[text];
			if(gText == NULL) {
				gText = new PhGraphicText( &_textFont, text->getContent());

				gText->setZ(-1);
				gText->init();

				_graphicTexts[text] = gText;
			}
			int track = text->getTrack();


			if( !((text->getTimeOut() < frameIn) || (text->getTimeIn() > frameOut)) ) {
				gText->setX(x + text->getTimeIn() * pixelPerFrame - offset);
				gText->setWidth((text->getTimeOut() - text->getTimeIn()) * pixelPerFrame);
				gText->setY(y + track * trackHeight);
				gText->setHeight(trackHeight);
				gText->setZ(-1);
				gText->setColor(computeColor(text->getPeople(), selectedPeoples, invertedColor));

				gText->draw();
			}

			PhStripText * lastText = lastTextList[track];
			// Display the people name only if one of the following condition is true:
			// - it is the first text
			// - it is a different people
			// - the distance between the latest text and the current is superior to a limit
			if((lastText == NULL) || (lastText->getPeople() != text->getPeople()) || (text->getTimeIn() - lastText->getTimeOut() > minSpaceBetweenPeople)) {
				PhPeople * people = text->getPeople();
				PhGraphicText * gPeople = _graphicPeoples[people];
				QString name = "???";
				if(people)
					name = people->getName();

				if(gPeople == NULL) {
					gPeople = new PhGraphicText(&_hudFont, name);
					gPeople->setWidth(name.length() * 12);
					gPeople->setZ(-1);

					gPeople->init();

					_graphicPeoples[people] = gPeople;
				}
				gPeople->setX(x + text->getTimeIn() * pixelPerFrame - offset - gPeople->getWidth() - spaceBetweenPeopleAndText);
				gPeople->setY(y + track * trackHeight);
				gPeople->setZ(-1);
				gPeople->setHeight(trackHeight / 2);

				gPeople->setColor(computeColor(people, selectedPeoples, invertedColor));

				gPeople->draw();

				//Check if the name is printed on the screen
				if( (frameIn < text->getTimeOut()) && (text->getTimeIn() - gPeople->getWidth() / pixelPerFrame < frameOut) ) {
					trackFull[track] = true;
				}

			}

			if(displayNextText && (frameOut < text->getTimeIn()) && ((lastText == NULL) || (text->getTimeIn() - lastText->getTimeOut() > minSpaceBetweenPeople))) {
				PhPeople * people = text->getPeople();
				QString name = "???";
				if(people)
					name = people->getName();

				PhGraphicText * gPeople = _graphicPeoples[people];
				if(gPeople == NULL) {
					gPeople = new PhGraphicText(&_textFont, name);
					gPeople->setWidth(name.length() * 12);

					gPeople->init();

					_graphicPeoples[people] = gPeople;
				}
				int howFarIsText = (text->getTimeIn() - frameOut) * verticalPixelPerFrame;
				//This line is used to see which text's name will be displayed
				gPeople->setX(width - gPeople->getWidth());
				gPeople->setY(y - howFarIsText);
				gPeople->setZ(-3);
				gPeople->setHeight(trackHeight / 2);

				gPeople->setColor(computeColor(people, selectedPeoples, invertedColor));


				PhGraphicSolidRect background(gPeople->getX(), gPeople->getY(), gPeople->getWidth(), gPeople->getHeight() + 2);
				if(selectedPeoples.size() && !selectedPeoples.contains(people))
					background.setColor(QColor(90, 90, 90));
				else
					background.setColor(QColor(180, 180, 180));

				background.setZ(gPeople->getZ() - 1);
				if(!invertedColor)
					background.draw();

				gPeople->draw();
			}

			lastTextList[track] = text;

			if(text->getTimeIn() > maxFrameIn)
				break;
		}

		delete lastTextList;

		foreach(PhStripCut * cut, _doc.getCuts())
		{
			//_counter++;
			if( (frameIn < cut->getTimeIn()) && (cut->getTimeIn() < frameOut)) {
				PhGraphicSolidRect *gCut = _graphicCuts[cut];
				if(gCut == NULL) {
					gCut = new PhGraphicSolidRect();
					gCut->setZ(-1);
					gCut->setWidth(2);

					_graphicCuts[cut] = gCut;
				}
				if(invertedColor)
					gCut->setColor(QColor(255, 255, 255));
				else
					gCut->setColor(QColor(0, 0, 0));
				gCut->setHeight(height);
				gCut->setX(x + cut->getTimeIn() * pixelPerFrame - offset);
				gCut->setY(y);

				gCut->draw();
				cutCounter++;
			}
			//Doesn't need to process undisplayed content
			if(cut->getTimeIn() > frameOut)
				break;
		}

		foreach(PhStripLoop * loop, _doc.getLoops())
		{
			//_counter++;
			// This calcul allow the cross to come smoothly on the screen (height / 8 /pixelPerFrame)
			if( ((loop->getTimeIn() + height / 8 /pixelPerFrame) > frameIn) && ((loop->getTimeIn() - height / 8 /pixelPerFrame ) < frameOut)) {
				PhGraphicLoop * gLoop = _graphicLoops[loop];
				if(gLoop == NULL) {
					gLoop = new PhGraphicLoop();
					_graphicLoops[loop] = gLoop;
					gLoop->setZ(-1);
				}
				if(!invertedColor)
					gLoop->setColor(Qt::black);
				else
					gLoop->setColor(Qt::white);

				gLoop->setX(x + loop->getTimeIn() * pixelPerFrame - offset);
				gLoop->setY(y);
				gLoop->setHThick(height / 40);
				gLoop->setHeight(height);
				gLoop->setCrossHeight(height / 4);
				gLoop->setWidth(height / 4);

				gLoop->draw();
				loopCounter++;
			}

			if(displayNextText && ((loop->getTimeIn() + height / 8 / pixelPerFrame) > frameIn)) {
				PhGraphicLoop gLoopPred;

				int howFarIsLoop = (loop->getTimeIn() - frameOut) * verticalPixelPerFrame;
				gLoopPred.setColor(Qt::white);

				gLoopPred.setHorizontalLoop(true);
				gLoopPred.setZ(-3);

				gLoopPred.setX(width - width / 10);
				gLoopPred.setY(y - howFarIsLoop);
				gLoopPred.setHeight(30);

				gLoopPred.setHThick(3);
				gLoopPred.setCrossHeight(20);
				gLoopPred.setWidth(width / 10);

				gLoopPred.draw();
			}
			if((loop->getTimeIn() - height / 8 / pixelPerFrame ) > frameOut + 25 * 30)
				break;
		}

		foreach(PhStripDetect * detect, _doc.getDetects())
		{
			//_counter++;

			if( detect->off() && (frameIn < detect->getTimeOut()) && (detect->getTimeIn() < frameOut) ) {
				PhGraphicSolidRect *gDetect = _graphicDetects[detect];
				if(gDetect == NULL) {
					gDetect = new PhGraphicSolidRect();
					_graphicDetects[detect] = gDetect;
					gDetect->setZ(-1);
				}

				gDetect->setColor(computeColor(detect->getPeople(), selectedPeoples, invertedColor));

				gDetect->setX(x + detect->getTimeIn() * pixelPerFrame - offset);
				gDetect->setY(y + detect->getTrack() * trackHeight + trackHeight * 0.8);
				gDetect->setHeight(trackHeight / 20);
				gDetect->setWidth((detect->getTimeOut() - detect->getTimeIn()) * pixelPerFrame);
				gDetect->draw();
				offCounter++;
			}
			//Doesn't need to process undisplayed content
			if(detect->getTimeIn() > frameOut)
				break;
		}
	}

	//	PHDEBUG << "off counter : " << offCounter << "cut counter : " << cutCounter << "loop counter : " << loopCounter;

	int currentDrawElapsed = _testTimer.elapsed() - lastDrawElapsed;
	if(_testTimer.elapsed() > 20)
		PHDEBUG << "Drop detected:" << ++_dropDetected;
	if(currentDrawElapsed > _maxDrawElapsed)
		_maxDrawElapsed = currentDrawElapsed;
	_testTimer.restart();

	if(_settings->displayStripInfo()) {
		int inc = 60;
		PhGraphicText text(&_hudFont, "", 0, 0, 200, inc);
		text.setColor(Qt::red);
		text.setContent(QString("Drop : %1").arg(_dropDetected));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("Max : %1").arg(_maxDrawElapsed));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("Count : %1").arg(counter));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("people : %1").arg(_graphicPeoples.count()));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("text : %1").arg(_graphicTexts.count()));
		text.draw();
	}
}
