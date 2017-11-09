/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2017-2017 German Aerospace Center (DLR) and others.
// TraCI4J module
// Copyright (C) 2011 ApPeAL Group, Politecnico di Torino
/****************************************************************************/
//
//   This program and the accompanying materials
//   are made available under the terms of the Eclipse Public License v2.0
//   which accompanies this distribution, and is available at
//   http://www.eclipse.org/legal/epl-v20.html
//
/****************************************************************************/
/// @file    StatusResponse.java
/// @author  Enrico Gueli
/// @author  Mario Krumnow
/// @date    2011
/// @version $Id$
///
//
/****************************************************************************/
package it.polito.appeal.traci.protocol;

import java.io.IOException;

import de.tudresden.sumo.config.Constants;
import de.uniluebeck.itm.tcpip.Storage;

public class StatusResponse {
	private final int id;
	private final int result;
	private final String description;
	
	public StatusResponse(int id) {
		this(id, Constants.RTYPE_OK, "");
	}
	
	public StatusResponse(int id, int result, String description) {
		this.id = id;
		this.result = result;
		this.description = description;
	}
	
	public StatusResponse(Storage packet) throws IOException {
		int len = packet.readByte();
		if (len == 0)
			packet.readInt(); // length is ignored; we can derive it
		
		id = packet.readUnsignedByte();
		result = packet.readUnsignedByte();
		description = packet.readStringASCII();
	}

	public int id() {
		return id;
	}
	
	/**
	 * @return the result
	 */
	public int result() {
		return result;
	}

	/**
	 * @return the description
	 */
	public String description() {
		return description;
	}
	
	public void writeTo(Storage out) throws IOException {
		out.writeByte(0);
		out.writeInt(5+1+1+4+description.length());
		out.writeByte(id);
		out.writeByte(result);
		out.writeStringASCII(description);
	}
}
