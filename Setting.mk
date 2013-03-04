#################################################
# User settings.
#################################################
export motion = IID
export scheduler = TransGroup
export network = CellularTorus
export packet = SinglePacket
export traffic = PermutationalPoisson
export node = HHRfNode
export logger = Printer


#################################################
# User setting candidates.
#################################################
export motion_all = IID RandomWalk RandomWaypoint HybridRandomWalk
export scheduler_all = TransGroup
export network_all = CellularTorus
export packet_all = SinglePacket
export traffic_all = PermutationalPoisson
export node_all = HHRfNode HHRfDNode
export logger_all = Printer Data
