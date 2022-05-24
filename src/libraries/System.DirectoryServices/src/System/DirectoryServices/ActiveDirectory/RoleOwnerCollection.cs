// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Collections;
using System.ComponentModel;

namespace System.DirectoryServices.ActiveDirectory
{
    public class ActiveDirectoryRoleCollection : ReadOnlyCollectionBase
    {
        internal ActiveDirectoryRoleCollection() { }

        internal ActiveDirectoryRoleCollection(ArrayList values)
        {
            if (values != null)
            {
                InnerList.AddRange(values);
            }
        }

        public ActiveDirectoryRole this[int index] => (ActiveDirectoryRole)InnerList[index]!;

        public bool Contains(ActiveDirectoryRole role)
        {
            //ArgumentOutOfRangeException.ThrowIfNotBetween(role, ActiveDirectoryRole.SchemaRole, ActiveDirectoryRole.InfrastructureRole);

            for (int i = 0; i < InnerList.Count; i++)
            {
                int tmp = (int)InnerList[i]!;
                if (tmp == (int)role)
                {
                    return true;
                }
            }
            return false;
        }

        public int IndexOf(ActiveDirectoryRole role)
        {
            //ArgumentOutOfRangeException.ThrowIfNotBetween(role, ActiveDirectoryRole.SchemaRole, ActiveDirectoryRole.InfrastructureRole);

            for (int i = 0; i < InnerList.Count; i++)
            {
                int tmp = (int)InnerList[i]!;

                if (tmp == (int)role)
                {
                    return i;
                }
            }

            return -1;
        }

        public void CopyTo(ActiveDirectoryRole[] roles, int index)
        {
            InnerList.CopyTo(roles, index);
        }
    }

    public class AdamRoleCollection : ReadOnlyCollectionBase
    {
        internal AdamRoleCollection() { }

        internal AdamRoleCollection(ArrayList values)
        {
            if (values != null)
            {
                InnerList.AddRange(values);
            }
        }

        public AdamRole this[int index] => (AdamRole)InnerList[index]!;

        public bool Contains(AdamRole role)
        {
            //ArgumentOutOfRangeException.ThrowIfNotBetween(role, AdamRole.SchemaRole, AdamRole.NamingRole);

            for (int i = 0; i < InnerList.Count; i++)
            {
                int tmp = (int)InnerList[i]!;
                if (tmp == (int)role)
                {
                    return true;
                }
            }
            return false;
        }

        public int IndexOf(AdamRole role)
        {
            //ArgumentOutOfRangeException.ThrowIfNotBetween(role, AdamRole.SchemaRole, AdamRole.NamingRole);

            for (int i = 0; i < InnerList.Count; i++)
            {
                int tmp = (int)InnerList[i]!;

                if (tmp == (int)role)
                {
                    return i;
                }
            }

            return -1;
        }

        public void CopyTo(AdamRole[] roles, int index)
        {
            InnerList.CopyTo(roles, index);
        }
    }
}
