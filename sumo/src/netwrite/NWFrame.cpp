/****************************************************************************/
/// @file    NWFrame.cpp
/// @author  Daniel Krajzewicz
/// @date    Tue, 20 Nov 2001
/// @version $Id$
///
// Sets and checks options for netwrite
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <utils/options/Option.h>
#include <utils/options/OptionsCont.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/SystemFrame.h>
#include <utils/iodevices/OutputDevice.h>
#include <netbuild/NBNetBuilder.h>
#include "NWFrame.h"
#include "NWWriter_SUMO.h"
#include "NWWriter_MATSim.h"
#include "NWWriter_XML.h"
#include "NWWriter_OpenDrive.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS

// ===========================================================================
// static members
// ===========================================================================
const std::string NWFrame::MAJOR_VERSION = "version=\"0.13\"";
const std::string NWFrame::XML_PARAMS = " encoding=\"UTF-8\"";


// ===========================================================================
// method definitions
// ===========================================================================
void
NWFrame::fillOptions() {
    OptionsCont& oc = OptionsCont::getOptions();
    // register options
    oc.doRegister("output-file", 'o', new Option_FileName());
    oc.addSynonyme("output-file", "sumo-output");
    oc.addSynonyme("output-file", "output");
    oc.addDescription("output-file", "Output", "The generated net will be written to FILE");

    oc.doRegister("plain-output-prefix", new Option_FileName());
    oc.addSynonyme("plain-output-prefix", "plain-output");
    oc.addDescription("plain-output-prefix", "Output", "Prefix of files to write plain xml nodes, edges and connections to");

    oc.doRegister("map-output", 'M', new Option_FileName());
    oc.addDescription("map-output", "Output", "Writes joined edges information to FILE");

    oc.doRegister("matsim-output", new Option_FileName());
    oc.addDescription("matsim-output", "Output", "The generated net will be written to FILE using MATsim format.");

    oc.doRegister("opendrive-output", new Option_FileName());
    oc.addDescription("opendrive-output", "Output", "The generated net will be written to FILE using openDRIVE format.");

    oc.doRegister("output.street-names", new Option_Bool(false));
    oc.addDescription("output.street-names", "Output", "Street names will be included in the output (if available).");
}


bool
NWFrame::checkOptions() {
    OptionsCont& oc = OptionsCont::getOptions();
    bool ok = true;
    // check whether the output is valid and can be build
    if (!oc.isSet("output-file") && !oc.isSet("plain-output-prefix") && !oc.isSet("matsim-output")) {
        oc.set("output-file", "net.net.xml");
    }
    return ok;
}


void
NWFrame::writeNetwork(const OptionsCont& oc, NBNetBuilder& nb) {
    NWWriter_SUMO::writeNetwork(oc, nb);
    NWWriter_MATSim::writeNetwork(oc, nb);
    NWWriter_OpenDrive::writeNetwork(oc, nb);
    NWWriter_XML::writeNetwork(oc, nb);
    // save the mapping information when wished
    if (oc.isSet("map-output")) {
        OutputDevice& mdevice = OutputDevice::getDevice(oc.getString("map-output"));
        mdevice << nb.getJoinedEdgesMap();
        mdevice.close();
    }
}


void
NWFrame::writePositionLong(const Position& pos, OutputDevice& dev) {
    dev.writeAttr(SUMO_ATTR_X, pos.x());
    dev.writeAttr(SUMO_ATTR_Y, pos.y());
    if (pos.z() != 0) {
        dev.writeAttr(SUMO_ATTR_Z, pos.z());
    }
}

/****************************************************************************/

