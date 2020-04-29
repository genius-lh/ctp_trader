create table strategy_config
(
	ConfigKey varchar(20),
	ConfigValue varchar(40),
	ConfigDesc varchar(100),
	PRIMARY KEY(ConfigKey)
);

create table strategy_instrument
(
	InstrumentID varchar(30) not null,
	ExchangeID varchar(8) not null,
	InstrumentName varchar(60) not null,
	ExchangeInstID varchar(30) not null,
	ProductID varchar(30) not null,
	ProductClass char(1) not null,
	DeliveryYear int not null,
	DeliveryMonth int not null,
	MaxMarketOrderVolume int not null,
	MinMarketOrderVolume int not null,
	MaxLimitOrderVolume int not null,
	MinLimitOrderVolume int not null,
	VolumeMultiple int not null,
	PriceTick double not null,
	CreateDate char(8) not null,
	OpenDate char(8) not null,
	ExpireDate char(8) not null,
	StartDelivDate char(8) not null,
	EndDelivDate char(8) not null,
	InstLifePhase char(1) not null,
	IsTrading int not null,
	PositionType char(1) not null,
	PositionDateType char(1) not null,
	LongMarginRatio double not null,
	ShortMarginRatio double not null,
	MaxMarginSideAlgorithm char(1) not null,
	UnderlyingInstrID char(30) not null,
	StrikePrice double not null,
	OptionsType char(1) not null,
	UnderlyingMultiple double not null,
	CombinationType char(1) not null,
	PRIMARY KEY(InstrumentID, ExchangeID)
);

create table strategy_market_data_tick_01
(
	InstrumentID varchar(30) not null,
	ExchangeID varchar(8) not null,
	TradingDay char(8) not null,
	UpdateTime char(8) not null,
	UpdateMillisec int not null,
	BidPrice1 double not null,
	AskPrice1 double not null,
	BidVolume1 int not null,
	AskVolume1 int not null,
	PRIMARY KEY(InstrumentID, ExchangeID, TradingDay, UpdateTime, UpdateMillisec)
);

create table strategy_market_data_min_01
(
	InstrumentID varchar(30) not null,
	ExchangeID varchar(8) not null,
	TradingDay char(8) not null,
	UpdateMinute char(4) not null,	
	OpenPrice double not null,
	HighestPrice double not null,
	LowestPrice double not null,
	ClosePrice double not null,
	PRIMARY KEY(InstrumentID, ExchangeID, TradingDay, UpdateMinute)
)