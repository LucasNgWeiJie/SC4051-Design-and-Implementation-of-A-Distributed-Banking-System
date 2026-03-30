#include "Service1_OpenAccountBoundary.h"
#include "Service1_OpenAccountControl.h"
#include "Service2_CloseAccountBoundary.h"
#include "Service2_CloseAccountControl.h"
#include "Service3_DepositWithdrawBoundary.h"
#include "Service3_DepositWithdrawControl.h"
#include "Service4_CheckBalanceBoundary.h"
#include "Service4_CheckBalanceControl.h"
#include "Service5_TransferBoundary.h"
#include "Service5_TransferControl.h"
#include "Service6_MonitorUpdatesBoundary.h"
#include "Service6_MonitorUpdatesControl.h"
#include "UdpTransport.h"
#include <iostream>
#include <string>

void print_menu()
{
  std::cout << "\n=== Distributed Banking System ===\n";
  std::cout << "1. Open Account\n2. Close Account\n3. Deposit / Withdraw\n";
  std::cout
      << "4. Check Balance\n5. Transfer\n6. Monitor Updates\n0. Exit\nChoice: ";
}

int main(int argc, char *argv[])
{
  // Check if configuration arguments were provided
  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0]
              << " <server_ip> <port> <semantics: 0=AtLeastOnce, 1=AtMostOnce> "
                 "[drop_rate]\n";
    return 1;
  }

  float drop_rate = 0.0f;
  if (argc >= 5)
  {
    drop_rate = std::stof(argv[4]);
  }

  // Bind transport using arguments provided
  UdpTransport transport(argv[1], std::stoi(argv[2]),
                         static_cast<Semantics>(std::stoi(argv[3])), drop_rate);

  // Initialize required services
  Service1_OpenAccountControl c1(&transport);
  Service1_OpenAccountBoundary b1(&c1);
  Service2_CloseAccountControl c2(&transport);
  Service2_CloseAccountBoundary b2(&c2);
  Service3_DepositWithdrawControl c3(&transport);
  Service3_DepositWithdrawBoundary b3(&c3);
  Service4_CheckBalanceControl c4(&transport);
  Service4_CheckBalanceBoundary b4(&c4);
  Service5_TransferControl c5(&transport);
  Service5_TransferBoundary b5(&c5);
  Service6_MonitorUpdatesControl c6(&transport);
  Service6_MonitorUpdatesBoundary b6(&c6);

  int choice;
  while (true)
  {
    print_menu();
    if (!(std::cin >> choice))
      break;
    switch (choice)
    {
    case 1:
      b1.execute();
      break;
    case 2:
      b2.execute();
      break;
    case 3:
      b3.execute();
      break;
    case 4:
      b4.execute();
      break;
    case 5:
      b5.execute();
      break;
    case 6:
      b6.execute();
      break;
    case 0:
      return 0;
    default:
      std::cout << "Invalid\n";
    }
  }
  return 0;
}
