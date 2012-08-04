/*
 * Copyright 2012 Rui Araújo, Luís Fonseca
 *
 * This file is part of Router Keygen.
 *
 * Router Keygen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Router Keygen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Router Keygen.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "alicehandler.h"
#include <iostream>
#include <stdio.h>

bool AliceHandler::isSupported() {
	return !this->supportedAlice.isEmpty();
}

QMap<QString, QVector<AliceMagicInfo *> *> AliceHandler::getSupportedAlice() {
	return this->supportedAlice;
}

bool AliceHandler::startElement(const QString & , const QString & ,
		const QString & qName, const QXmlAttributes &attributes) {
	int magic[2];
	QString serial;
	QString mac;
	bool status;
	if (attributes.count() == 0)
		return true;
	QVector<AliceMagicInfo *> * supported = supportedAlice.value(qName);
	if (supported == 0) {
		supported = new QVector<AliceMagicInfo *>();
		supportedAlice.insert(qName, supported);
	}
	serial = attributes.value("sn");
	mac = attributes.value("mac");
	magic[0] = attributes.value("q").toInt(&status, 10);
	magic[1] = attributes.value("k").toInt(&status, 10);
	supported->append(new AliceMagicInfo(qName, magic, serial, mac));

	return true;
}

bool AliceHandler::fatalError(const QXmlParseException &exception) {
	std::cerr << "Parse error at line " << exception.lineNumber() << ", "
			<< "column " << exception.columnNumber() << ": "
			<< qPrintable(exception.message()) << std::endl;
	return false;
}
bool AliceHandler::readFile(const QString &fileName) {
	QFile file(fileName);
	QXmlInputSource inputSource(&file);
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	return reader.parse(inputSource);
}
