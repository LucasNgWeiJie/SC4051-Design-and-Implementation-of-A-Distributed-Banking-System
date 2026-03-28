CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude

SRCS = src/main.cpp src/utils/UdpTransport.cpp src/boundary/Service1_OpenAccountBoundary.cpp src/boundary/Service2_CloseAccountBoundary.cpp src/boundary/Service3_DepositWithdrawBoundary.cpp src/boundary/Service4_CheckBalanceBoundary.cpp src/boundary/Service5_TransferBoundary.cpp src/boundary/Service6_MonitorUpdatesBoundary.cpp src/control/Service1_OpenAccountControl.cpp src/control/Service2_CloseAccountControl.cpp src/control/Service3_DepositWithdrawControl.cpp src/control/Service4_CheckBalanceControl.cpp src/control/Service5_TransferControl.cpp src/control/Service6_MonitorUpdatesControl.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = client_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
