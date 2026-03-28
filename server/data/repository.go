package data

import (
	"bank_server/common"
	"errors"
	"sync"
)

type AccountRepository struct {
	accounts      map[int32]*common.Account
	nextAccountID int32
	mu            sync.Mutex
}

func NewAccountRepository() *AccountRepository {
	return &AccountRepository{
		accounts:      make(map[int32]*common.Account),
		nextAccountID: 1000,
	}
}

func (r *AccountRepository) CreateAccount(name, password string, currency uint8, balance float32) int32 {
	r.mu.Lock()
	defer r.mu.Unlock()
	
	// Generate new account ID
	id := r.nextAccountID
	r.nextAccountID++
	
	// Store new account in map
	r.accounts[id] = &common.Account{
		Number:   id,
		Name:     name,
		Password: password,
		Currency: currency,
		Balance:  balance,
	}
	return id
}

func (r *AccountRepository) DeleteAccount(id int32, name, password string) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	acc, ok := r.accounts[id]
	if !ok {
		return errors.New("Account not found")
	}
	if acc.Name != name || acc.Password != password {
		return errors.New("Authentication failed")
	}
	delete(r.accounts, id)
	return nil
}

func (r *AccountRepository) GetAccount(id int32, name, password string) (*common.Account, error) {
	r.mu.Lock()
	defer r.mu.Unlock()
	acc, ok := r.accounts[id]
	if !ok {
		return nil, errors.New("Account not found")
	}
	if acc.Name != name || acc.Password != password {
		return nil, errors.New("Authentication failed")
	}
	return acc, nil
}

func (r *AccountRepository) UpdateBalance(id int32, name, password string, currency uint8, amount float32) (float32, error) {
	r.mu.Lock()
	defer r.mu.Unlock()
	acc, ok := r.accounts[id]
	if !ok {
		return 0, errors.New("Account not found")
	}
	if acc.Name != name || acc.Password != password {
		return 0, errors.New("Authentication failed")
	}
	if acc.Currency != currency {
		return 0, errors.New("Currency mismatch")
	}
	if acc.Balance+amount < 0 {
		return 0, errors.New("Insufficient funds")
	}
	acc.Balance += amount
	return acc.Balance, nil
}

func (r *AccountRepository) Transfer(fromId int32, fromName, fromPass string, toId int32, amount float32) (float32, error) {
	r.mu.Lock()
	defer r.mu.Unlock()
	
	// Validate transfer amount
	if amount <= 0 {
		return 0, errors.New("Invalid transfer amount")
	}
	
	// Authenticate source account
	fromAcc, ok := r.accounts[fromId]
	if !ok || fromAcc.Name != fromName || fromAcc.Password != fromPass {
		return 0, errors.New("Authentication failed")
	}
	
	// Verify destination account
	toAcc, ok := r.accounts[toId]
	if !ok {
		return 0, errors.New("Destination account not found")
	}
	
	// Validate currencies match
	if fromAcc.Currency != toAcc.Currency {
		return 0, errors.New("Currency mismatch")
	}
	
	// Check sufficient balance
	if fromAcc.Balance < amount {
		return 0, errors.New("Insufficient funds")
	}
	
	// Execute transfer
	fromAcc.Balance -= amount
	toAcc.Balance += amount
	return fromAcc.Balance, nil
}
