#################################################
# User settings.
#################################################
export motion = IID
export scheduler = TransGroup
export network = CellularTorus
export packet = SinglePacket
export traffic = PermutationalGeometric
export node = HHRtauNode
export logger = HHRObserver


#################################################
# User setting candidates.
#################################################
export motion_all = IID
export scheduler_all = TransGroup
export network_all = CellularTorus
export packet_all = SinglePacket
export traffic_all = PermutationalPoisson PermutationalGeometric
export node_all = HHRfNode HHRftNode HHRtauNode
export logger_all = Printer Data HHRObserver
