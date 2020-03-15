-- MySQL dump 10.10
--
-- Host: localhost    Database: db_mrbm
-- ------------------------------------------------------
-- Server version	5.0.20a-nt

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tb_base_book_kind`
--

DROP TABLE IF EXISTS `tb_base_book_kind`;
CREATE TABLE `tb_base_book_kind` (
  `bookkind` varchar(25) NOT NULL,
  PRIMARY KEY  USING BTREE (`bookkind`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 PACK_KEYS=1;

--
-- Dumping data for table `tb_base_book_kind`
--


/*!40000 ALTER TABLE `tb_base_book_kind` DISABLE KEYS */;
LOCK TABLES `tb_base_book_kind` WRITE;
INSERT INTO `tb_base_book_kind` VALUES ('心里学'),('文学'),('管理学'),('计算机');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_base_book_kind` ENABLE KEYS */;

--
-- Table structure for table `tb_base_desk_info`
--

DROP TABLE IF EXISTS `tb_base_desk_info`;
CREATE TABLE `tb_base_desk_info` (
  `deskname` varchar(25) NOT NULL,
  PRIMARY KEY  (`deskname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_base_desk_info`
--


/*!40000 ALTER TABLE `tb_base_desk_info` DISABLE KEYS */;
LOCK TABLES `tb_base_desk_info` WRITE;
INSERT INTO `tb_base_desk_info` VALUES ('1号柜台'),('2号柜台'),('3号柜台'),('超级柜台');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_base_desk_info` ENABLE KEYS */;

--
-- Table structure for table `tb_base_store_info`
--

DROP TABLE IF EXISTS `tb_base_store_info`;
CREATE TABLE `tb_base_store_info` (
  `storename` varchar(25) NOT NULL,
  PRIMARY KEY  USING BTREE (`storename`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_base_store_info`
--


/*!40000 ALTER TABLE `tb_base_store_info` DISABLE KEYS */;
LOCK TABLES `tb_base_store_info` WRITE;
INSERT INTO `tb_base_store_info` VALUES ('东1仓库'),('北1仓库'),('南1仓库'),('南2仓库'),('店内'),('西1仓库');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_base_store_info` ENABLE KEYS */;

--
-- Table structure for table `tb_book_adjust`
--

DROP TABLE IF EXISTS `tb_book_adjust`;
CREATE TABLE `tb_book_adjust` (
  `adjustcode` int(25) NOT NULL auto_increment,
  `operator` varchar(25) NOT NULL,
  `take` varchar(25) NOT NULL,
  `store` varchar(25) NOT NULL,
  `barcode` varchar(25) NOT NULL,
  `provider` varchar(25) NOT NULL,
  `count` int(11) NOT NULL,
  `date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`adjustcode`),
  KEY `FK_tb_book_adjust_1` (`barcode`),
  KEY `FK_tb_book_adjust_2` (`operator`),
  KEY `FK_tb_book_adjust_3` (`provider`),
  KEY `FK_tb_book_adjust_4` (`store`),
  CONSTRAINT `FK_tb_book_adjust_1` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_adjust_2` FOREIGN KEY (`operator`) REFERENCES `tb_operators` (`name`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_adjust_3` FOREIGN KEY (`provider`) REFERENCES `tb_provider_info` (`provider`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_adjust_4` FOREIGN KEY (`store`) REFERENCES `tb_base_store_info` (`storename`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_book_adjust`
--


/*!40000 ALTER TABLE `tb_book_adjust` DISABLE KEYS */;
LOCK TABLES `tb_book_adjust` WRITE;
INSERT INTO `tb_book_adjust` VALUES (18,'mrsoft','北1仓库','店内','IBN987-6543-21','XXX出版商',10,'2013-02-03 23:25:33');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_adjust` ENABLE KEYS */;

--
-- Table structure for table `tb_book_info`
--

DROP TABLE IF EXISTS `tb_book_info`;
CREATE TABLE `tb_book_info` (
  `barcode` varchar(25) NOT NULL,
  `bookname` varchar(25) NOT NULL,
  `mncode` varchar(25) NOT NULL,
  `authorname` varchar(25) NOT NULL,
  `bookconcern` varchar(25) NOT NULL,
  `price` double NOT NULL,
  `memo` varchar(25) NOT NULL default '无',
  `kind` varchar(25) NOT NULL,
  PRIMARY KEY  (`barcode`),
  KEY `FK_tb_book_info_1` (`kind`),
  CONSTRAINT `FK_tb_book_info_1` FOREIGN KEY (`kind`) REFERENCES `tb_base_book_kind` (`bookkind`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_book_info`
--


/*!40000 ALTER TABLE `tb_book_info` DISABLE KEYS */;
LOCK TABLES `tb_book_info` WRITE;
INSERT INTO `tb_book_info` VALUES ('IBN987-6543-21','WINAPI控件指南','123','松鼠','mr',33.33,'无','计算机');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_info` ENABLE KEYS */;

--
-- Table structure for table `tb_book_input`
--

DROP TABLE IF EXISTS `tb_book_input`;
CREATE TABLE `tb_book_input` (
  `inputcode` int(25) unsigned NOT NULL auto_increment,
  `operator` varchar(25) NOT NULL,
  `provider` varchar(25) NOT NULL,
  `barcode` varchar(25) NOT NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `store` varchar(25) NOT NULL,
  `count` int(25) unsigned NOT NULL,
  `pay` double NOT NULL,
  PRIMARY KEY  USING BTREE (`inputcode`,`provider`,`store`),
  KEY `FK_tb_book_input_3` (`provider`),
  KEY `FK_tb_book_input_2` (`barcode`),
  KEY `FK_tb_book_input_1` (`store`),
  KEY `FK_tb_book_input_4` (`operator`),
  CONSTRAINT `FK_tb_book_input_1` FOREIGN KEY (`store`) REFERENCES `tb_base_store_info` (`storename`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_2` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_3` FOREIGN KEY (`provider`) REFERENCES `tb_provider_info` (`provider`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_4` FOREIGN KEY (`operator`) REFERENCES `tb_operators` (`name`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 3072 kB; ';

--
-- Dumping data for table `tb_book_input`
--


/*!40000 ALTER TABLE `tb_book_input` DISABLE KEYS */;
LOCK TABLES `tb_book_input` WRITE;
INSERT INTO `tb_book_input` VALUES (5,'mrsoft','XXX出版商','IBN987-6543-21','2013-02-04 07:18:14','东1仓库',24,1066.56),(6,'mrsoft','XXX出版商','IBN987-6543-21','2013-02-03 23:05:39','北1仓库',14,466.62),(7,'mrsoft','XXX出版商','IBN987-6543-21','2013-02-03 23:05:49','北1仓库',14,466.62);
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_input` ENABLE KEYS */;

--
-- Table structure for table `tb_book_input_back`
--

DROP TABLE IF EXISTS `tb_book_input_back`;
CREATE TABLE `tb_book_input_back` (
  `backcode` int(25) NOT NULL auto_increment,
  `operator` varchar(25) NOT NULL,
  `provider` varchar(25) NOT NULL,
  `barcode` varchar(25) NOT NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `store` varchar(25) NOT NULL,
  `count` int(25) NOT NULL,
  `pay` double NOT NULL,
  PRIMARY KEY  (`backcode`),
  KEY `FK_tb_book_input_back_1` (`barcode`),
  KEY `FK_tb_book_input_back_2` (`store`),
  KEY `FK_tb_book_input_back_3` (`provider`),
  KEY `FK_tb_book_input_back_4` (`operator`),
  CONSTRAINT `FK_tb_book_input_back_1` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_back_2` FOREIGN KEY (`store`) REFERENCES `tb_base_store_info` (`storename`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_back_3` FOREIGN KEY (`provider`) REFERENCES `tb_provider_info` (`provider`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_input_back_4` FOREIGN KEY (`operator`) REFERENCES `tb_operators` (`name`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_book_input_back`
--


/*!40000 ALTER TABLE `tb_book_input_back` DISABLE KEYS */;
LOCK TABLES `tb_book_input_back` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_input_back` ENABLE KEYS */;

--
-- Table structure for table `tb_book_sale`
--

DROP TABLE IF EXISTS `tb_book_sale`;
CREATE TABLE `tb_book_sale` (
  `salecode` int(25) unsigned NOT NULL auto_increment,
  `operator` varchar(25) NOT NULL,
  `barcode` varchar(25) NOT NULL,
  `desk` varchar(25) NOT NULL,
  `customer` varchar(25) NOT NULL default '无记名',
  `count` int(25) unsigned NOT NULL,
  `date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `pay` double NOT NULL,
  PRIMARY KEY  (`salecode`),
  KEY `FK_tb_book_sale_1` (`desk`),
  KEY `FK_tb_book_sale_2` (`barcode`),
  KEY `FK_tb_book_sale_3` (`operator`),
  CONSTRAINT `FK_tb_book_sale_1` FOREIGN KEY (`desk`) REFERENCES `tb_base_desk_info` (`deskname`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_sale_2` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_sale_3` FOREIGN KEY (`operator`) REFERENCES `tb_operators` (`name`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_book_sale`
--


/*!40000 ALTER TABLE `tb_book_sale` DISABLE KEYS */;
LOCK TABLES `tb_book_sale` WRITE;
INSERT INTO `tb_book_sale` VALUES (8,'mrsoft','IBN987-6543-21','1号柜台','无记名',3,'2013-02-03 23:31:38',99.99);
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_sale` ENABLE KEYS */;

--
-- Table structure for table `tb_book_sale_back`
--

DROP TABLE IF EXISTS `tb_book_sale_back`;
CREATE TABLE `tb_book_sale_back` (
  `backcode` int(25) NOT NULL auto_increment,
  `operator` varchar(25) NOT NULL,
  `barcode` varchar(25) NOT NULL,
  `desk` varchar(25) NOT NULL,
  `customer` varchar(25) NOT NULL default '无记名',
  `count` int(25) NOT NULL,
  `date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `pay` double NOT NULL,
  PRIMARY KEY  (`backcode`),
  KEY `FK_tb_book_sale_back_1` (`barcode`),
  KEY `FK_tb_book_sale_back_2` (`desk`),
  KEY `FK_tb_book_sale_back_3` (`operator`),
  CONSTRAINT `FK_tb_book_sale_back_1` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_sale_back_2` FOREIGN KEY (`desk`) REFERENCES `tb_base_desk_info` (`deskname`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_book_sale_back_3` FOREIGN KEY (`operator`) REFERENCES `tb_operators` (`name`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 3072 kB; (`desk`) REFER `db_mrbm/tb_base_desk_i';

--
-- Dumping data for table `tb_book_sale_back`
--


/*!40000 ALTER TABLE `tb_book_sale_back` DISABLE KEYS */;
LOCK TABLES `tb_book_sale_back` WRITE;
INSERT INTO `tb_book_sale_back` VALUES (3,'mrsoft','IBN987-6543-21','1号柜台','无记名',3,'2013-02-03 23:34:33',99.99);
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_book_sale_back` ENABLE KEYS */;

--
-- Table structure for table `tb_operators`
--

DROP TABLE IF EXISTS `tb_operators`;
CREATE TABLE `tb_operators` (
  `name` varchar(25) NOT NULL,
  `password` varchar(25) character set utf8 collate utf8_bin NOT NULL default '111',
  `level` int(10) NOT NULL default '1',
  PRIMARY KEY  (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 PACK_KEYS=1;

--
-- Dumping data for table `tb_operators`
--


/*!40000 ALTER TABLE `tb_operators` DISABLE KEYS */;
LOCK TABLES `tb_operators` WRITE;
INSERT INTO `tb_operators` VALUES ('employee','123',1),('mrsoft','111',2);
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_operators` ENABLE KEYS */;

--
-- Table structure for table `tb_provider_info`
--

DROP TABLE IF EXISTS `tb_provider_info`;
CREATE TABLE `tb_provider_info` (
  `provider` varchar(25) NOT NULL,
  `corporation` varchar(25) NOT NULL,
  `principal` varchar(25) NOT NULL,
  `phone` varchar(25) NOT NULL,
  `addr` varchar(25) NOT NULL,
  `web` varchar(25) NOT NULL,
  `e_mail` varchar(25) NOT NULL,
  PRIMARY KEY  (`provider`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_provider_info`
--


/*!40000 ALTER TABLE `tb_provider_info` DISABLE KEYS */;
LOCK TABLES `tb_provider_info` WRITE;
INSERT INTO `tb_provider_info` VALUES ('XXX出版商','老赵','老李','1234567890','长春市高新开发区','http//www........','lx@mrbccd.com');
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_provider_info` ENABLE KEYS */;

--
-- Table structure for table `tb_stock_info`
--

DROP TABLE IF EXISTS `tb_stock_info`;
CREATE TABLE `tb_stock_info` (
  `barcode` varchar(25) NOT NULL,
  `store` varchar(25) NOT NULL,
  `stock` int(25) unsigned NOT NULL default '0',
  KEY `FK_db_stock_info_1` (`barcode`),
  KEY `FK_db_stock_info_2` (`store`),
  CONSTRAINT `FK_db_stock_info_1` FOREIGN KEY (`barcode`) REFERENCES `tb_book_info` (`barcode`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_db_stock_info_2` FOREIGN KEY (`store`) REFERENCES `tb_base_store_info` (`storename`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `tb_stock_info`
--


/*!40000 ALTER TABLE `tb_stock_info` DISABLE KEYS */;
LOCK TABLES `tb_stock_info` WRITE;
INSERT INTO `tb_stock_info` VALUES ('IBN987-6543-21','北1仓库',18),('IBN987-6543-21','南1仓库',0),('IBN987-6543-21','南2仓库',0),('IBN987-6543-21','店内',10),('IBN987-6543-21','西1仓库',0),('IBN987-6543-21','东1仓库',24);
UNLOCK TABLES;
/*!40000 ALTER TABLE `tb_stock_info` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

