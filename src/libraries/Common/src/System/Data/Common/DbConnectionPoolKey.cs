// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

namespace System.Data.Common
{
    // DbConnectionPoolKey: Base class implementation of a key to connection pool groups
    //  Only connection string is used as a key
    internal sealed class DbConnectionPoolKey : ICloneable
    {
        private string? _connectionString;

        internal DbConnectionPoolKey(string? connectionString)
        {
            _connectionString = connectionString;
        }

        private DbConnectionPoolKey(DbConnectionPoolKey key)
        {
            _connectionString = key.ConnectionString;
        }

        public object Clone()
        {
            return new DbConnectionPoolKey(this);
        }

        internal string? ConnectionString
        {
            get
            {
                return _connectionString;
            }

            set
            {
                _connectionString = value;
            }
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            if (obj == null || obj.GetType() != typeof(DbConnectionPoolKey))
            {
                return false;
            }

            DbConnectionPoolKey? key = obj as DbConnectionPoolKey;

            return (key != null && _connectionString == key._connectionString);
        }

        public override int GetHashCode()
        {
            return _connectionString == null ? 0 : _connectionString.GetHashCode();
        }
    }
}
