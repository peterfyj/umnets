#################################################
# User settings.
#################################################
export motion = IID
export scheduler = TransGroup
export network = CellularTorus
export packet = SinglePacket
export traffic = PermutationalPoisson
export node = HHRftNode
export logger = ServiceObserver


#################################################
# User setting candidates.
#################################################
export motion_all = IID
export scheduler_all = TransGroup
export network_all = CellularTorus
export packet_all = SinglePacket
export traffic_all = PermutationalPoisson
export node_all = HHRfNode HHRftNode
export logger_all = Printer Data ServiceObserver
