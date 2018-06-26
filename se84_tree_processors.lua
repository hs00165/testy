require("nscl_unpacker/nscl_unpacker_cfg")
local mapping = require("se84_dp/se84_mapping")
local calib = require("se84_dp/se84_calibration")

LoadLib("se84_detclasses_cxx.so", "se84_detclasses")

local tbranches, tree, SIDAR_buf, BarrelUp_buf, BarrelDown_buf, Elastics_buf, SIDAR_hits, BarrelUp_hits, BarrelDown_hits, Elastics_hits

function Initialization(input_type)
  tree = TTree("se84", "84Se (d,p)")

  tbranches = {}
  tbranches.SIDAR = tree:NewBranch("SIDAR", "vector<SIDAR_detclass>")

  SIDAR_buf = {}
  for i=1,6 do SIDAR_buf[i] = SIDAR_detclass(); SIDAR_buf[i]:Set("detID", i) end

  BarrelUp_buf = {}
  for i=1,12 do BarrelUp_buf[i] = Barrel_detclass(); BarrelUp_buf[i]:Set("detID", i) end

  BarrelDown_buf = {}
  for i=1,12 do BarrelDown_buf[i] = Barrel_detclass(); BarrelDown_buf[i]:Set("detID", i) end

  Elastics_buf = {}
  for i=1,3 do Elastics_buf[i] = Barrel_detclass(); Elastics_buf[i]:Set("detID", i) end

  tbranches.BarrelUp = tree:NewBranch("BarrelUp", "vector<Barrel_detclass>")

  tbranches.BarrelDown = tree:NewBranch("BarrelDown", "vector<Barrel_detclass>")

  tbranches.Elastics = tree:NewBranch("Elastics", "vector<Barrel_detclass>")

  tbranches.IonChamber = tree:NewBranch("IonChamber", "IonChamber_detclass")

  tbranches.CRDC = {}
  tbranches.CRDC[1] = tree:NewBranch("CRDC1", "CRDC_detclass")
  tbranches.CRDC[2] = tree:NewBranch("CRDC2", "CRDC_detclass")

  tbranches.MTDC = tree:NewBranch("MTDC", "MTDC_detclass")

  tbranches.Scintillators = tree:NewBranch("Scintillators", "Scintillators_detclass")

  tbranches.MCP1 = tree:NewBranch("MCP1", "Generic_detclass")
  tbranches.MCP2 = tree:NewBranch("MCP2", "Generic_detclass")

  tbranches.TDC = tree:NewBranch("TDC", "Generic_detclass")

  tbranches.EVTSCALERS = tree:NewBranch("EventScalers", "Generic_detclass")

  tbranches.trig = tree:NewBranch("trig", "short")

  tbranches.ts_s800 = tree:NewBranch("ts_s800", "unsigned long long")
  tbranches.ts_ORRUBA = tree:NewBranch("ts_ORRUBA", "unsigned long long")

  return tree, tbranches
end

---------------------------------------------------------------
----------------------- PROCESSORS ----------------------------
---------------------------------------------------------------


local CRDCProcessor = {
  anode = function(crdcnum, time)
    tbranches.CRDC[crdcnum]:Set("time", time)
  end,

  raw = function(crdcnum, pad, en_avg, data)
--    tbranches.CRDC[crdcnum]:Get("pads"):PushBack(pad)
--    tbranches.CRDC[crdcnum]:Get("raw"):PushBack(data.energies)
--    tbranches.CRDC[crdcnum]:Get("sample_nbr"):PushBack(data.samples)
    tbranches.CRDC[crdcnum]:Get("average_raw"):Set(en_avg)
  end,

  cal = function(crdcnum, mult, xgravity)
    tbranches.CRDC[crdcnum]:Get("xgrav"):Set(xgravity)
  end,
}

local ScintProcessor = {
  up = function(val)
    tbranches.Scintillators:Get("up"):PushBack(val)
  end,

  down = function(val)
    tbranches.Scintillators:Get("down"):PushBack(val)
  end,
}

local IonChamberProcessor = {
  matrix = function(channel, value)
    tbranches.IonChamber:Get("pads"):PushBack(channel)
    tbranches.IonChamber:Get("energies"):PushBack(value)
  end,

  average = function(val)
    tbranches.IonChamber:Set("average_energy", val)
  end,
}

local MTDCProcessor = {
  matrix = function(chname, value)
    tbranches.MTDC:Get(chname.."_hits"):PushBack(value)
  end,
}

local TimestampProcessor = function(val, src)
  if src == 2 then tbranches.ts_s800:Set(val) end
  if src == 16 then tbranches.ts_ORRUBA:Set(val) end
end

local ORRUBAFillFns = {}

ORRUBAFillFns.SIDAR = function(detinfo, val)
  local detnum = detinfo.detnum

  if not SIDAR_hits[detnum] then
    SIDAR_hits[detnum] = true
    SIDAR_buf[detnum]:Reset()
  end

  if detinfo.detpos == "dE" then
    SIDAR_buf[detnum]:Get("dE_strips"):PushBack(detinfo.stripnum)
    SIDAR_buf[detnum]:Get("dE_energies"):PushBack(val)
  else
    SIDAR_buf[detnum]:Get("E_strips"):PushBack(detinfo.stripnum)
    SIDAR_buf[detnum]:Get("E_energies"):PushBack(val)
  end
end

ORRUBAFillFns.BB10 = function(detinfo, val)
  local detnum = detinfo.detnum

  if not BarrelUp_hits[detnum] then
    BarrelUp_hits[detnum] = true
    BarrelUp_buf[detnum]:Reset()
  end

  BarrelUp_buf[detnum]:Get("dE_strips"):PushBack(detinfo.stripnum)
  BarrelUp_buf[detnum]:Get("dE_energies"):PushBack(val)
end

superX3Ufn = function(detinfo, val)
  local detnum = detinfo.detnum

  if not BarrelUp_hits[detnum] then
    BarrelUp_hits[detnum] = true
    BarrelUp_buf[detnum]:Reset()
  end

  if detinfo.detside == "front" then
    BarrelUp_buf[detnum]:Get("E_front_contacts"):PushBack(detinfo.stripnum)
    BarrelUp_buf[detnum]:Get("E_front_energies"):PushBack(val)
  else
    BarrelUp_buf[detnum]:Get("E_back_strips"):PushBack(detinfo.stripnum)
    BarrelUp_buf[detnum]:Get("E_back_energies"):PushBack(val)
  end
end

superX3Dfn = function(detinfo, val)
  local detnum = detinfo.detnum

  if not BarrelDown_hits[detnum] then
    BarrelDown_hits[detnum] = true
    BarrelDown_buf[detnum]:Reset()
  end

  if detinfo.detside == "front" then
    BarrelDown_buf[detnum]:Get("E_front_contacts"):PushBack(detinfo.stripnum)
    BarrelDown_buf[detnum]:Get("E_front_energies"):PushBack(val)
  else
    BarrelDown_buf[detnum]:Get("E_back_strips"):PushBack(detinfo.stripnum)
    BarrelDown_buf[detnum]:Get("E_back_energies"):PushBack(val)
  end
end

ORRUBAFillFns.SuperX3 = function(detinfo, val)
  if detinfo.detpos == "U" then
    superX3Ufn(detinfo, val)
  else
    superX3Dfn(detinfo, val)
  end
end

ORRUBAFillFns.X3 = function(detinfo, val)
  local detnum = detinfo.detnum

  if not BarrelDown_hits[detnum] then
    BarrelDown_hits[detnum] = true
    BarrelDown_buf[detnum]:Reset()
  end

  BarrelDown_buf[detnum]:Get("dE_strips"):PushBack(detinfo.stripnum)
  BarrelDown_buf[detnum]:Get("dE_energies"):PushBack(val)
end

ORRUBAFillFns.Elastics = function(detinfo, val)
  local detnum = detinfo.detnum

  if not Elastics_hits[detnum] then
    Elastics_hits[detnum] = true
    Elastics_buf[detnum]:Reset()
  end

  if detinfo.detside == "front" then
    Elastics_buf[detnum]:Get("E_front_contacts"):PushBack(detinfo.stripnum)
    Elastics_buf[detnum]:Get("E_front_energies"):PushBack(val)
  end
end

ORRUBAFillFns.MCP = function(detinfo, val)
  local mcp_branch = detinfo.detnum == 1 and tbranches.MCP1 or tbranches.MCP2
  mcp_branch:Get("channels"):PushBack(detinfo.channel)
  mcp_branch:Get("values"):PushBack(val)
end

ORRUBAFillFns.TDC = function(detinfo, val)
  tbranches.TDC:Get("channels"):PushBack(detinfo.channel)
  tbranches.TDC:Get("values"):PushBack(val)
end

ORRUBAFillFns.EVTSCALERS = function(detinfo, val)
  tbranches.EVTSCALERS:Get("channels"):PushBack(detinfo.channel)
  tbranches.EVTSCALERS:Get("values"):PushBack(val)
end

local ORRUBAProcessor = function(orruba_data)
  SIDAR_hits = {}
  BarrelUp_hits = {}
  BarrelDown_hits = {}
  Elastics_hits = {}

  for k, v in pairs(orruba_data) do
    local detinfo = mapping.getdetinfo(k)

    if detinfo then
      local fillfn = ORRUBAFillFns[detinfo.dettype]
      if fillfn then fillfn(detinfo, v) end
    else
--      print("no detinfo for channel", k)
    end
  end

  for k, v in pairs(SIDAR_hits) do
    tbranches.SIDAR:PushBack(SIDAR_buf[k])
  end

  for k, v in pairs(BarrelUp_hits) do
    tbranches.BarrelUp:PushBack(BarrelUp_buf[k])
  end

  for k, v in pairs(BarrelDown_hits) do
    tbranches.BarrelDown:PushBack(BarrelDown_buf[k])
  end

  for k, v in pairs(Elastics_hits) do
    tbranches.Elastics:PushBack(Elastics_buf[k])
  end
end

local TriggerProcessor = function(trig)
  tbranches.trig:Set(trig)
end

NSCL_UNPACKER.SetCRDCProcessor(CRDCProcessor)
--NSCL_UNPACKER.SetHodoProcessor()
NSCL_UNPACKER.SetScintProcessor(ScintProcessor)
NSCL_UNPACKER.SetIonChamberProcessor(IonChamberProcessor)
NSCL_UNPACKER.SetMTDCProcessor(MTDCProcessor)
NSCL_UNPACKER.SetTriggerProcessor(TriggerProcessor)
--NSCL_UNPACKER.SetTOFProcessor()
NSCL_UNPACKER.SetORRUBAProcessor(ORRUBAProcessor)
NSCL_UNPACKER.TriggerProcessor = TriggerProcessor
NSCL_UNPACKER.TimestampProcessor = TimestampProcessor

NSCL_UNPACKER.SetPostProcessing(function(buf)
--    tbranches.timestamp:Set(buf.timestamp)
    tree:Fill()
    tree:Reset()
  end)


return Initialization