#################################################
# User settings.
#################################################
motion = IID
scheduler = TransGroup
network = CellularTorus
pack = SinglePack
traffic = PermutationalPoisson
node = HHRfNode
logger = Printer


#################################################
# User setting candidates.
# Use this for reference, but DO NOT EDIT.
#################################################
# IID: random cell selection.
# 	No parameter.
# RandomWalk: random walk the adjacent nine cells.
#   $1: pace.  Number of time slots between moves.
# RandomWaypoint: random walk with offset (ox, oy) where ox and oy are
# uniformly drawn from [-3, -1] union [1, 3].
#   $1: pace.  Number of time slots between moves.
# HybridRandomWalk: suppose there are totally m * n squares in the network, the 
# network is divided into m^beta1 * n^beta2 cells, and each cell is further 
# divided into m^(0.5-beta1) * n^(0.5-beta2) subcells. The node is going to take
# a move every pace timeslots by jumping to a random subcell of one of the 
# four adjacent cells.
#   $1: pace.  Number of time slots between moves.
# 	$2: m^beta1.  Must be a divisor of m^0.5.
# 	$3: n^beta2.  Must be a divisor of n^0.5.
motion_all = IID RandomWalk RandomWaypoint HybridRandomWalk

# TransGroup: each node in the transmission group sends.
# 	$1: alpha of x coordinate.  Network should be exactly divided.
# 	$2: alpha of y coordinate.  Network should be exactly divided.
scheduler_all = TransGroup

# CellularTorus: local transmission, one pack a time.
# 	$1: the size of the torus.  There are $1 * $1 cells.
network_all = CellularTorus

# SinglePack: Pack with a single information unit.
# 	No parameter.
pack_all = SinglePack

# PermutationalPoisson: Permutational traffic, Poisson pack generation.
# 	$1: lambda of the Poisson stream, relative to a time slot.
traffic_all = PermutationalPoisson

# $1: Should always be the number of nodes.
#
# HHRfNode: 2HR-f node.
# 	$1: derived.  Number of nodes.
# 	$2: f.
# HHRfDNode: 2HR-fD node.
# 	$1: derived.  Number of nodes.
# 	$2: f.
# 	$3: d.
node_all = HHRfNode HHRfDNode

# Printer: print information to standard output.
#   No parameter.
# Data: collect data for some certain analysis.
#   No parameter.
logger_all = Printer Data

# Global parameter:
# 	-loop: number of timeslots to simulate.
