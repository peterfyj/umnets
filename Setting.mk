#################################################
# User settings.
#################################################
export motion = IID
export scheduler = TransGroup
export network = CellularTorus
export packet = SinglePacket
export traffic = PermutationalPoisson
export node = HHRtkpNode
export logger = Printer


#################################################
# User setting candidates.
#################################################
export motion_all = IID
export scheduler_all = TransGroup
export network_all = CellularTorus
export packet_all = SinglePacket
export traffic_all = PermutationalPoisson
export node_all = HHRfNode HHRftNode HHRtkpNode
export logger_all = Printer Data HHRObserver
